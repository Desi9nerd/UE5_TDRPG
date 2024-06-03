#include "UI/Widget/Inventory/TDUW_InventoryEntry.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UTDUW_InventoryEntry::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	UTDInventorySlot* Item = Cast<UTDInventorySlot>(ListItemObject);
	Image_Item->SetBrushFromTexture(Item->InventorySlot.Item.Thumbnail);
	Text_ItemQuantity->SetText(FText::AsNumber(Item->InventorySlot.ItemQuantity));
}
