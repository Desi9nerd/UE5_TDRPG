#include "UI/Widget/Inventory/TDUW_InventoryToolTip.h"
#include "Components/TextBlock.h"
#include "Library/TDItemLibrary.h"
#include "UI/Widget/Inventory/TDUW_InventoryEntry.h"

void UTDUW_InventoryToolTip::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(InventorySlotBeingHovered->GetFItemReference().Thumbnail))
	{
		const FItem& ItemBeingHovered = InventorySlotBeingHovered->GetFItemReference();
		ItemName->SetText(ItemBeingHovered.Name);
		//ItemQuantity->SetText(InventorySlotBeingHovered->Text_ItemQuantity->GetText());
		ItemDescription->SetText(ItemBeingHovered.Description);

		SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		SetVisibility(ESlateVisibility::Hidden);
	}

}
