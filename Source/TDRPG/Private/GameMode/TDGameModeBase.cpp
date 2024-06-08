#include "GameMode/TDGameModeBase.h"
#include "SaveGame/TDSaveGame_Load.h"
#include "Kismet/GameplayStatics.h"
#include "UI/MVVM/TDMVVM_Slot.h"

void ATDGameModeBase::SaveSlotData(UTDMVVM_Slot* LoadSlot, int32 SlotIndex)
{
	if (UGameplayStatics::DoesSaveGameExist(LoadSlot->LoadSlotName, SlotIndex)) // 이름이 같은 SaveGame이 있다면
	{
		UGameplayStatics::DeleteGameInSlot(LoadSlot->LoadSlotName, SlotIndex); // 해당 SaveGame 삭제.
	}

	USaveGame* SaveGameObject = UGameplayStatics::CreateSaveGameObject(LoadScreenSaveGameClass); // SaveGame 생성.
	UTDSaveGame_Load* LoadScreenSaveGame = Cast<UTDSaveGame_Load>(SaveGameObject);
	LoadScreenSaveGame->PlayerName = LoadSlot->PlayerName; // 캐릭터 이름 설정.
	LoadScreenSaveGame->SaveSlotStatus = Taken; // 슬롯상태를 Taken으로 설정.

	//* 게임 저장
	UGameplayStatics::SaveGameToSlot(LoadScreenSaveGame, LoadSlot->LoadSlotName, SlotIndex);
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
