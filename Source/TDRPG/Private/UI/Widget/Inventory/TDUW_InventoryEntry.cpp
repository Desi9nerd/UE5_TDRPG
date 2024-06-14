#include "UI/Widget/Inventory/TDUW_InventoryEntry.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UTDUW_InventoryEntry::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	UTDInventorySlot* Item = Cast<UTDInventorySlot>(ListItemObject);

	if (Item == nullptr)
	{
		Image_Item->SetVisibility(ESlateVisibility::Hidden);
		Text_ItemQuantity->SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	Image_Item->SetBrushFromTexture(Item->InventorySlot.Item.Thumbnail);
	Text_ItemQuantity->SetText(FText::AsNumber(Item->InventorySlot.ItemQuantity));
}

void UTDUW_InventoryEntry::UpdateInventorySlotUI(const FItem& InItem, int32 InItemQuantity)
{
	if (InItemQuantity == 0)
	{
		Image_Item->SetVisibility(ESlateVisibility::Hidden);
		Text_ItemQuantity->SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	Image_Item->SetBrushFromTexture(InItem.Thumbnail);
	Image_Item->SetVisibility(ESlateVisibility::HitTestInvisible);
	Text_ItemQuantity->SetText(FText::AsNumber(InItemQuantity));
	if (InItemQuantity > 1)
	{
		Text_ItemQuantity->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
	{
		Text_ItemQuantity->SetVisibility(ESlateVisibility::Hidden);
	}
}
