#include "UI/Widget/Inventory/TDUW_InventoryToolTip.h"

#include "Actor/TDItemActor.h"
#include "Components/TextBlock.h"

void UTDUW_InventoryToolTip::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(InventorySlotBeingHovered->GetFItemReference().ItemClass))
	{
		const FItem& ItemBeingHovered = InventorySlotBeingHovered->GetFItemReference();
		ItemName->SetText(ItemBeingHovered.Name);
		//ItemQuantity->SetText(InventorySlotBeingHovered->Text_ItemQuantity->GetText());
		ItemDescription->SetText(ItemBeingHovered.Description);
	}
}
