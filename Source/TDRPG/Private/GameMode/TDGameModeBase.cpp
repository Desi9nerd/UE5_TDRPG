#include "GameMode/TDGameModeBase.h"
#include "GameInstance/TDGameInstance.h"
#include "SaveGame/TDSaveGame_Load.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "UI/MVVM/TDMVVM_Slot.h"

void ATDGameModeBase::SaveSlotData(UTDMVVM_Slot* LoadSlot, int32 SlotIndex)
{
	if (UGameplayStatics::DoesSaveGameExist(LoadSlot->GetLoadSlotName(), SlotIndex)) // 이름이 같은 SaveGame이 있다면
	{
		UGameplayStatics::DeleteGameInSlot(LoadSlot->GetLoadSlotName(), SlotIndex); // 해당 SaveGame 삭제.
	}

	USaveGame* SaveGameObject = UGameplayStatics::CreateSaveGameObject(LoadScreenSaveGameClass); // SaveGame 생성.
	UTDSaveGame_Load* LoadScreenSaveGame = Cast<UTDSaveGame_Load>(SaveGameObject);
	LoadScreenSaveGame->PlayerName = LoadSlot->GetPlayerName(); // 캐릭터 이름 설정.
	LoadScreenSaveGame->MapName = LoadSlot->GetMapName(); // 맵 이름 설정.
	LoadScreenSaveGame->PlayerStartTag = LoadSlot->PlayerStartTag; // PlayerStartTag 설정.
	LoadScreenSaveGame->SaveSlotStatus = Taken; // 슬롯상태를 Taken으로 설정.

	//* 게임 저장
	UGameplayStatics::SaveGameToSlot(LoadScreenSaveGame, LoadSlot->GetLoadSlotName(), SlotIndex);
}

UTDSaveGame_Load* ATDGameModeBase::GetSaveSlotData(const FString& SlotName, int32 SlotIdx) const
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

	UTDSaveGame_Load* LoadScreenSaveGame = Cast<UTDSaveGame_Load>(SaveGameObject);

	return LoadScreenSaveGame;
}

void ATDGameModeBase::DeleteSlot(const FString& SlotName, int32 SlotIndex)
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, SlotIndex)) // SlotName에 해당되는 SaveGame이 있다면
	{
		UGameplayStatics::DeleteGameInSlot(SlotName, SlotIndex); // 해당 SaveGame(=슬롯) 삭제.
	}
}

void ATDGameModeBase::TravelToMap(UTDMVVM_Slot* Slot)
{
	const FString SlotName = Slot->GetLoadSlotName();
	const int32 SlotIndex = Slot->SlotIndex;

	//* Maps변수에 해당 맵이 있는지 확인 후 있다면 맵을 연다.
	UGameplayStatics::OpenLevelBySoftObjectPtr(Slot, Maps.FindChecked(Slot->GetMapName()));
}

AActor* ATDGameModeBase::ChoosePlayerStart_Implementation(AController* Player)
{
	UTDGameInstance* TDGameInstance = Cast<UTDGameInstance>(GetGameInstance());

	TArray<AActor*> StartActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), StartActors); // GetWorld()에 있는 PlayerStarts들을 StartActors 배열변수에 다 담는다.

	if (StartActors.Num() > 0)
	{
		AActor* SelectedActor = StartActors[0];
		for (AActor* Actor : StartActors)
		{
			if (APlayerStart* PlayerStart = Cast<APlayerStart>(Actor))
			{
				if (PlayerStart->PlayerStartTag == TDGameInstance->PlayerStartTag) // PlayerStartTag 태그가 일치한다면
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

void ATDGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	Maps.Add(DefaultMapName, DefaultMap); // 기본맵 등록.
}
