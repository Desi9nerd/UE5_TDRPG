#include "UI/Widget/Inventory/TDUW_InventorySlot.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

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

void UTDUW_InventorySlot::UpdateInventorySlotUI(FItem& InItem, int32 InItemQuantity)
{
	if (InItemQuantity == 0)
	{
		Image_Item->SetVisibility(ESlateVisibility::Hidden);
		Text_ItemQuantity->SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	Image_Item->SetBrushFromTexture(InItem.Thumbnail);
	Image_Item->SetVisibility(ESlateVisibility::SelfHitTestInvisible); // 보이지만 활성화X
	Text_ItemQuantity->SetText(FText::AsNumber(InItemQuantity));
	if (InItemQuantity > 1)
	{
		Text_ItemQuantity->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		Text_ItemQuantity->SetVisibility(ESlateVisibility::Hidden);
	}
}
