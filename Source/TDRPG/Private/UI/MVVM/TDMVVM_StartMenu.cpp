#include "UI/MVVM/TDMVVM_StartMenu.h"
#include "GameMode/TDGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "UI/MVVM/TDMVVM_Slot.h"

void UTDMVVM_StartMenu::InitializeLoadSlots()
{
	//* Slot 생성 후 초기값 넣어줌.
	LoadSlot_0 = NewObject<UTDMVVM_Slot>(this, LoadSlotViewModelClass);
	LoadSlot_0->SetLoadSlotName(FString("LoadSlot_0"));
	LoadSlot_0->SlotIndex = 0;
	LoadSlots.Add(0, LoadSlot_0);

	LoadSlot_1 = NewObject<UTDMVVM_Slot>(this, LoadSlotViewModelClass);
	LoadSlot_1->SetLoadSlotName(FString("LoadSlot_1"));
	LoadSlot_1->SlotIndex = 1;
	LoadSlots.Add(1, LoadSlot_1);

	LoadSlot_2 = NewObject<UTDMVVM_Slot>(this, LoadSlotViewModelClass);
	LoadSlot_2->SetLoadSlotName(FString("LoadSlot_2"));
	LoadSlot_2->SlotIndex = 2;
	LoadSlots.Add(2, LoadSlot_2);

	SetNumLoadSlots(LoadSlots.Num());
}

void UTDMVVM_StartMenu::SetNumLoadSlots(int32 InNumLoadSlots)
{
	UE_MVVM_SET_PROPERTY_VALUE(NumLoadSlots, InNumLoadSlots);
}

UTDMVVM_Slot* UTDMVVM_StartMenu::GetLoadSlotViewModelByIndex(int32 Index) const
{
	return LoadSlots.FindChecked(Index);
}

void UTDMVVM_StartMenu::NewSlotButtonPressed(int32 Slot, const FString& EnteredName)
{
	ATDGameModeBase* TDGameMode = Cast<ATDGameModeBase>(UGameplayStatics::GetGameMode(this));

	LoadSlots[Slot]->SetPlayerName(EnteredName); // 플레이어 이름 설정.
	LoadSlots[Slot]->SetMapName(TDGameMode->DefaultMapName); // 맵 이름 설정.
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

	SelectedSlot = LoadSlots[Slot]; // 선택한 슬롯 설정.
}

void UTDMVVM_StartMenu::DeleteButtonPressed()
{
	if (IsValid(SelectedSlot))
	{
		ATDGameModeBase::DeleteSlot(SelectedSlot->GetLoadSlotName(), SelectedSlot->SlotIndex); // 해당 슬롯 삭제.
		SelectedSlot->SlotStatus = Vacant; // 삭제 후 해당 슬롯상태를 Vacant로 설정.
		SelectedSlot->InitializeSlot();	// 삭제 후 해당 슬롯 초기화.
		SelectedSlot->EnableSelectSlotButton.Broadcast(true); // 삭제 후 해당 슬롯 버튼상태를 true로 Broadcast.
	}
}

void UTDMVVM_StartMenu::PlayButtonPressed()
{
	ATDGameModeBase* TDGameMode = Cast<ATDGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (IsValid(SelectedSlot))
	{
		TDGameMode->TravelToMap(SelectedSlot); // 선택한 슬롯의 맵 열기.
	}
}

void UTDMVVM_StartMenu::LoadData()
{
	ATDGameModeBase* TDGameMode = Cast<ATDGameModeBase>(UGameplayStatics::GetGameMode(this));

	for (const TTuple<int32, UTDMVVM_Slot*> LoadSlot : LoadSlots)
	{
		UTDSaveGame_Load* SaveObject = TDGameMode->GetSaveSlotData(LoadSlot.Value->GetLoadSlotName(), LoadSlot.Key);
		
		//* UTDMVVM_Slot 내의 SlotStatus, PlayerName 설정.
		LoadSlot.Value->SlotStatus = SaveObject->SaveSlotStatus;; // 슬롯상태 설정.
		LoadSlot.Value->SetPlayerName(SaveObject->PlayerName); // 플레이어 이름 설정.
		LoadSlot.Value->SetMapName(SaveObject->MapName); // 맵 이름 설정.
		LoadSlot.Value->InitializeSlot(); // 슬롯 Broadcast.
	}
}
