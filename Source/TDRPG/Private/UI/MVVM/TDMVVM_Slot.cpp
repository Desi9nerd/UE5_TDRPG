#include "UI/MVVM/TDMVVM_Slot.h"

void UTDMVVM_Slot::InitializeSlot()
{
	const int32 WidgetSwitcherIdx = SlotStatus.GetValue(); // 슬롯상태Enum을 Byte값으로 받아서 인덱스로 사용. 

	SetWidgetSwitcherIdx.Broadcast(WidgetSwitcherIdx);
}

void UTDMVVM_Slot::SetPlayerName(FString InPlayerName)
{
	// MVVM 시스템 매크로. PlayerName을 InPlayerName로 설정.
	UE_MVVM_SET_PROPERTY_VALUE(PlayerName, InPlayerName);
}

void UTDMVVM_Slot::SetMapName(FString InMapName)
{
	UE_MVVM_SET_PROPERTY_VALUE(MapName, InMapName);
}

void UTDMVVM_Slot::SetLoadSlotName(FString InLoadSlotName)
{
	UE_MVVM_SET_PROPERTY_VALUE(LoadSlotName, InLoadSlotName);
}
