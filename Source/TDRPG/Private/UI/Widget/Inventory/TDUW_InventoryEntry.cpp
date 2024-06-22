#include "UI/Widget/Inventory/TDUW_InventoryEntry.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UTDUW_InventoryEntry::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	UTDInventoryDisplayItemObject* Item = Cast<UTDInventoryDisplayItemObject>(ListItemObject);

	if (Item == nullptr)
	{
		Image_Item->SetVisibility(ESlateVisibility::Hidden);
		Text_ItemQuantity->SetVisibility(ESlateVisibility::Hidden);
		return;
	}
		
	if (IsValid(Item->Data.Item.Thumbnail))
	{
		Image_Item->SetBrushFromTexture(Item->Data.Item.Thumbnail);
		Image_Item->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		Image_Item->SetVisibility(ESlateVisibility::Hidden);
	}

	Text_ItemQuantity->SetText(FText::AsNumber(Item->Data.ItemQuantity));
	if (Item->Data.ItemQuantity > 1)
	{
		Text_ItemQuantity->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
	{
		Text_ItemQuantity->SetVisibility(ESlateVisibility::Hidden);
	}
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
