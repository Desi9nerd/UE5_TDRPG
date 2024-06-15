#include "GameMode/TDGameModeBase_Single.h"
#include "EngineUtils.h"
#include "GameFramework/Character.h"
#include "GameInstance/TDGameInstance.h"
#include "SaveGame/TDSaveGame_Load.h"
#include "GameFramework/PlayerStart.h"
#include "Interface/ISaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "UI/MVVM/TDMVVM_Slot.h"

TObjectPtr<UTDGameInstance> ATDGameModeBase_Single::GetTDGameInstance()
{
	if (IsValid(TDGameInstance)) return TDGameInstance;

	TDGameInstance = Cast<UTDGameInstance>(GetGameInstance());
	return TDGameInstance;
}

void ATDGameModeBase_Single::SaveSlotData(UTDMVVM_Slot* LoadSlot, int32 SlotIndex)
{
	if (UGameplayStatics::DoesSaveGameExist(LoadSlot->GetLoadSlotName(), SlotIndex)) // 이름이 같은 SaveGame이 있다면
	{
		UGameplayStatics::DeleteGameInSlot(LoadSlot->GetLoadSlotName(), SlotIndex); // 해당 SaveGame 삭제.
	}

	USaveGame* SaveGameObject = UGameplayStatics::CreateSaveGameObject(LoadScreenSaveGameClass); // SaveGame 생성.
	UTDSaveGame_Load* LoadScreenSaveGame = Cast<UTDSaveGame_Load>(SaveGameObject);
	LoadScreenSaveGame->PlayerName = LoadSlot->GetPlayerName(); // 캐릭터 이름 설정.
	LoadScreenSaveGame->MapName = LoadSlot->GetMapName(); // 맵 이름 설정.
	LoadScreenSaveGame->MapAssetName = LoadSlot->GetMapAssetName(); // 맵 에셋 이름 설정.
	LoadScreenSaveGame->PlayerStartTag = LoadSlot->PlayerStartTag; // PlayerStartTag 설정.
	LoadScreenSaveGame->SaveSlotStatus = Taken; // 슬롯상태를 Taken으로 설정.

	//* 게임 저장
	UGameplayStatics::SaveGameToSlot(LoadScreenSaveGame, LoadSlot->GetLoadSlotName(), SlotIndex);
}

UTDSaveGame_Load* ATDGameModeBase_Single::GetSaveSlotData(const FString& SlotName, int32 SlotIdx) const
{
	USaveGame* SaveGameObject = nullptr;
	if (UGameplayStatics::DoesSaveGameExist(SlotName, SlotIdx)) // SlotName에 해당되는 이름의 게임이 있다면
	{
		SaveGameObject = UGameplayStatics::LoadGameFromSlot(SlotName, SlotIdx); // 해당 게임 로드.
	}
	else // 없다면
	{
		SaveGameObject = UGameplayStatics::CreateSaveGameObject(LoadScreenSaveGameClass); // 게임 생성.
	}

	UTDSaveGame_Load* TDSaveGame_Load = Cast<UTDSaveGame_Load>(SaveGameObject);

	return TDSaveGame_Load;
}

void ATDGameModeBase_Single::DeleteSlot(const FString& SlotName, int32 SlotIndex)
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, SlotIndex)) // SlotName에 해당되는 SaveGame이 있다면
	{
		UGameplayStatics::DeleteGameInSlot(SlotName, SlotIndex); // 해당 SaveGame(=슬롯) 삭제.
	}
}

// 'LoadSlotName, LoadSlotIndex'가 일치하는 저장된 게임을 가져옴. 
UTDSaveGame_Load* ATDGameModeBase_Single::RetrieveInGameSaveData()
{
	const FString InGameLoadSlotName = GetTDGameInstance()->LoadSlotName;
	const int32 InGameLoadSlotIndex = GetTDGameInstance()->LoadSlotIndex;

	return GetSaveSlotData(InGameLoadSlotName, InGameLoadSlotIndex);
}

// SaveObject를 'LoadSlotName, LoadSlotIndex, PlayerStartTag' 정보와 매칭시켜 저장.
void ATDGameModeBase_Single::SaveInGameProgressData(UTDSaveGame_Load* SaveObject)
{
	const FString InGameLoadSlotName = GetTDGameInstance()->LoadSlotName;
	const int32 InGameLoadSlotIndex = GetTDGameInstance()->LoadSlotIndex;
	GetTDGameInstance()->PlayerStartTag = SaveObject->PlayerStartTag;

	UGameplayStatics::SaveGameToSlot(SaveObject, InGameLoadSlotName, InGameLoadSlotIndex);
}

// SaveMap에 월드 정보를 저장하는 함수.
void ATDGameModeBase_Single::SaveWorldState(UWorld* World)
{
	FString WorldName = World->GetMapName();
	WorldName.RemoveFromStart(World->StreamingLevelsPrefix); // 접두어 제거.

	checkf(GetTDGameInstance(), TEXT("No TDGameInstance. Check ATDGameModeBase::SaveWorldState()"));

	UTDSaveGame_Load* SaveGame = GetSaveSlotData(GetTDGameInstance()->LoadSlotName, GetTDGameInstance()->LoadSlotIndex);
	if (IsValid(SaveGame))
	{
		//***************************************************************************
		//* 새로운 데이터 넣기
		if (false == SaveGame->HasMap(WorldName)) // WorldName에 해당하는 맵이 없다면
		{
			// WorldName이름의 맵 생성 후 SavedMaps 배열에 추가.
			FSavedMap NewSavedMap;
			NewSavedMap.MapAssetName = WorldName;
			SaveGame->SavedMaps.Add(NewSavedMap);
		}

		FSavedMap SavedMap = SaveGame->GetSavedMapWithMapName(WorldName);
		SavedMap.SavedActors.Empty(); // 배열 비우기. 아래에서 Actor들을 채운다.

		// FActorIterator 사용 시 #include "EngineUtils.h" 필요.
		for (FActorIterator It(World); It; ++It) // 월드에 있는 Actor들.
		{
			AActor* Actor = *It;
			IISaveGame* SaveGameInterface = Cast<IISaveGame>(Actor);

			if (false == IsValid(Actor) || nullptr == SaveGameInterface) continue;

			FSavedActor SavedActor;
			SavedActor.ActorName = Actor->GetFName();
			SavedActor.Transform = Actor->GetTransform();

			FMemoryWriter MemoryWriter(SavedActor.Bytes);

			FObjectAndNameAsStringProxyArchive Archive(MemoryWriter, true);
			Archive.ArIsSaveGame = true;

			Actor->Serialize(Archive); // Archive에 Actor를 Serializing. 

			SavedMap.SavedActors.AddUnique(SavedActor); // SaveMap의 저장할 액터 배열에 SaveActor를 추가.
		}
		//***************************************************************************

		//***************************************************************************
		//* Old Data를 (지우고) SaveMap으로 대체하기
		for (FSavedMap& MapToReplace : SaveGame->SavedMaps)
		{
			if (MapToReplace.MapAssetName == WorldName)
			{
				MapToReplace = SavedMap;
			}
		}
		//***************************************************************************

		//* 게임 저장하기.
		UGameplayStatics::SaveGameToSlot(SaveGame, GetTDGameInstance()->LoadSlotName, GetTDGameInstance()->LoadSlotIndex);
	}
}

// SaveMap에 저장된 Actor 데이터를 로드하는 함수.
void ATDGameModeBase_Single::LoadWorldState(UWorld* World)
{
	FString WorldName = World->GetMapName();
	WorldName.RemoveFromStart(World->StreamingLevelsPrefix);

	checkf(GetTDGameInstance(), TEXT("No TDGameInstance. Check ATDGameModeBase::LoadWorldState()"));

	if (UGameplayStatics::DoesSaveGameExist(GetTDGameInstance()->LoadSlotName, GetTDGameInstance()->LoadSlotIndex))// 해당 게임이 있다면
	{
		UTDSaveGame_Load* SaveGame = Cast<UTDSaveGame_Load>(UGameplayStatics::LoadGameFromSlot(GetTDGameInstance()->LoadSlotName, GetTDGameInstance()->LoadSlotIndex));
		if (false == IsValid(SaveGame))
		{
			UE_LOG(LogTemp, Error, TEXT("슬롯을 로드하는데 실패했습니다."));
			return;
		}

		for (FActorIterator It(World); It; ++It) // 월드에 있는 Actor들.
		{
			AActor* Actor = *It;
			IISaveGame* SaveGameInterface = Cast<IISaveGame>(Actor);

			if (nullptr == SaveGameInterface) continue;

			for (FSavedActor SavedActor : SaveGame->GetSavedMapWithMapName(WorldName).SavedActors)
			{
				if (SavedActor.ActorName == Actor->GetFName())
				{
					if (SaveGameInterface->ShouldLoadTransform()) // TODO
					{
						Actor->SetActorTransform(SavedActor.Transform);
					}

					FMemoryReader MemoryReader(SavedActor.Bytes);

					FObjectAndNameAsStringProxyArchive Archive(MemoryReader, true);
					Archive.ArIsSaveGame = true;
					Actor->Serialize(Archive); // binary bytes를 변수로 변환.

					SaveGameInterface->LoadActor();
				}
			}
		}
	}
}

void ATDGameModeBase_Single::TravelToMap(UTDMVVM_Slot* Slot)
{
	const FString SlotName = Slot->GetLoadSlotName();
	const int32 SlotIndex = Slot->SlotIndex;

	//* Maps변수에 해당 맵이 있는지 확인 후 있다면 맵을 연다.
	UGameplayStatics::OpenLevelBySoftObjectPtr(Slot, Maps.FindChecked(Slot->GetMapName()));
}

AActor* ATDGameModeBase_Single::ChoosePlayerStart_Implementation(AController* Player)
{
	TArray<AActor*> StartActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), StartActors); // GetWorld()에 있는 PlayerStarts들을 StartActors 배열변수에 다 담는다.

	if (StartActors.Num() > 0)
	{
		AActor* SelectedActor = StartActors[0];
		for (AActor* Actor : StartActors)
		{
			if (APlayerStart* PlayerStart = Cast<APlayerStart>(Actor))
			{
				if (PlayerStart->PlayerStartTag == GetTDGameInstance()->PlayerStartTag) // PlayerStartTag 태그가 일치한다면
				{
					SelectedActor = PlayerStart;
					break;
				}
			}
		}

		return SelectedActor;
	}

	return nullptr;
}

void ATDGameModeBase_Single::PlayerDeath(ACharacter* DeadCharacter)
{
	UTDSaveGame_Load* TDSaveGame_Load = RetrieveInGameSaveData(); // 마지막으로 저장된 SaveGame 데이터.
	if (false == IsValid(TDSaveGame_Load)) return;

	UGameplayStatics::OpenLevel(DeadCharacter, FName(TDSaveGame_Load->MapAssetName));
}

void ATDGameModeBase_Single::BeginPlay()
{
	Super::BeginPlay();

	Maps.Add(DefaultMapName, DefaultMap); // 기본맵 등록.
}

