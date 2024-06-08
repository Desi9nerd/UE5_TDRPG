#include "UI/MVVM/TDMVVM_StartMenu.h"
#include "GameMode/TDGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "UI/MVVM/TDMVVM_Slot.h"

void UTDMVVM_StartMenu::InitializeLoadSlots()
{
	LoadSlot_0 = NewObject<UTDMVVM_Slot>(this, LoadSlotViewModelClass);
	LoadSlot_0->LoadSlotName = FString("LoadSlot_0");
	LoadSlots.Add(0, LoadSlot_0);
	LoadSlot_1 = NewObject<UTDMVVM_Slot>(this, LoadSlotViewModelClass);
	LoadSlot_1->LoadSlotName = FString("LoadSlot_1");
	LoadSlots.Add(1, LoadSlot_1);
	LoadSlot_2 = NewObject<UTDMVVM_Slot>(this, LoadSlotViewModelClass);
	LoadSlot_2->LoadSlotName = FString("LoadSlot_2");
	LoadSlots.Add(2, LoadSlot_2);
}

UTDMVVM_Slot* UTDMVVM_StartMenu::GetLoadSlotViewModelByIndex(int32 Index) const
{
	return LoadSlots.FindChecked(Index);
}

void UTDMVVM_StartMenu::NewSlotButtonPressed(int32 Slot, const FString& EnteredName)
{
	ATDGameModeBase* TDGameMode = Cast<ATDGameModeBase>(UGameplayStatics::GetGameMode(this));

	LoadSlots[Slot]->SetPlayerName(EnteredName); // 플레이어 이름 설정.
	LoadSlots[Slot]->SlotStatus = Taken; // 슬롯상태 Taken으로 설정.

	TDGameMode->SaveSlotData(LoadSlots[Slot], Slot); // SaveGame 생성 후 저장.
	LoadSlots[Slot]->InitializeSlot();
}

void UTDMVVM_StartMenu::NewGameButtonPressed(int32 Slot)
{
	LoadSlots[Slot]->SetWidgetSwitcherIdx.Broadcast(1);
}

void UTDMVVM_StartMenu::SelectSlotButtonPressed(int32 Slot)
{
	SlotSelected.Broadcast();

	for (const TTuple<int32, UTDMVVM_Slot*> LoadSlot : LoadSlots)
	{
		if (LoadSlot.Key == Slot) // 매개변수로 들어온 슬롯(=선택한 슬롯)이 기존의 슬롯들 중에 있다면
		{
			LoadSlot.Value->EnableSelectSlotButton.Broadcast(false); // false로 Broadcast.
		}
		else
		{
			LoadSlot.Value->EnableSelectSlotButton.Broadcast(true); // true로 Broadcast.
		}
	}
}

void UTDMVVM_StartMenu::LoadData()
{
	ATDGameModeBase* TDGameMode = Cast<ATDGameModeBase>(UGameplayStatics::GetGameMode(this));

	for (const TTuple<int32, UTDMVVM_Slot*> LoadSlot : LoadSlots)
	{
		UTDSaveGame_Load* SaveObject = TDGameMode->GetSaveSlotData(LoadSlot.Value->LoadSlotName, LoadSlot.Key);

		const FString PlayerName = SaveObject->PlayerName;
		TEnumAsByte<ESG_SaveSlotStatus> SaveSlotStatus = SaveObject->SaveSlotStatus;

		//* UTDMVVM_Slot 내의 SlotStatus, PlayerName 설정.
		LoadSlot.Value->SetPlayerName(PlayerName); // 플레이어 이름 설정.
		LoadSlot.Value->SlotStatus = SaveSlotStatus; // 슬롯상태 설정.
		LoadSlot.Value->InitializeSlot(); // 슬롯 Broadcast.
	}
}
