#include "UI/Widget/Inventory/TDUW_InventorySlot.h"

void UTDUW_InventorySlot::NativeConstruct()
{
	Super::NativeConstruct();

	if (false == IsValid(Image_Slot)) {
		UE_LOG(LogTemp, Warning, TEXT("No Image_Slot. Check UTDUW_InventorySlot"));
	}
	if (false == IsValid(Image_Item)) {
		UE_LOG(LogTemp, Warning, TEXT("No Image_Item. Check UTDUW_InventorySlot"));
	}
}
