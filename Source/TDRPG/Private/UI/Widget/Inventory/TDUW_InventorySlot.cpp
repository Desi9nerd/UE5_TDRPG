#include "UI/Widget/Inventory/TDUW_InventorySlot.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Character/TDCharacter.h"
#include "Component/TDInventoryComponent.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Player/TDPlayerController.h"
#include "UI/DragDropOperation/TDDragDropOperation.h"
#include "UI/HUD/TDHUD.h"
#include "UI/Widget/Inventory/TDUW_Inventory.h"
#include "UI/Widget/Inventory/TDUW_InventoryMenu.h"

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

void UTDUW_InventorySlot::UpdateInventorySlotUI(const FItem& InItem, int32 InItemQuantity)
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

FReply UTDUW_InventorySlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		UE_LOG(LogTemp, Warning, TEXT("Native On Mouse Button Down!! "));

		// FReply::Handled()를 반환하여 이벤트 처리를 했음을 시스템에 알림.
		return FReply::Handled().DetectDrag(TakeWidget(), EKeys::LeftMouseButton);
	}

	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent); // 다른 마우스 버튼 클릭은 기본 처리를 사용
}

void UTDUW_InventorySlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
	UE_LOG(LogTemp, Warning, TEXT("Inventory Slot NativeOn Drag Detected!! "));

	//****************************************************************************
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	ATDPlayerController* TDPlayerController = Cast<ATDPlayerController>(PlayerController);
	ATDCharacter* TDCharacter = Cast<ATDCharacter>(PlayerController->GetCharacter());
	
	TArray<FInventorySlot>* SelectedCategoryItems = nullptr;

	switch (TDCharacter->GetInventoryComponent()->GetSelectedInventoryCategory())
	{
	case EItemCategory::Weapon:
		SelectedCategoryItems = &TDCharacter->GetInventoryComponent()->GetWeaponCategory();
		break;
	case EItemCategory::Armor:
		SelectedCategoryItems = &TDCharacter->GetInventoryComponent()->GetArmorCategory();
		break;
	case EItemCategory::Potion:
		SelectedCategoryItems = &TDCharacter->GetInventoryComponent()->GetPotionCategory();
		break;
	case EItemCategory::Food:
		SelectedCategoryItems = &TDCharacter->GetInventoryComponent()->GetFoodCategory();
		break;
	default:
		break;
	}

	//****************************************************************************

	if ((*SelectedCategoryItems)[SlotIndex].ItemQuantity != 0)
	{
		UTDDragDropOperation* DragItemOperation = NewObject<UTDDragDropOperation>();
		DragItemOperation->DefaultDragVisual = Image_Item;

		OutOperation = DragItemOperation;

		TDPlayerController->GetTDHUD()->GetInventoryWidget()->DraggedSlotIndex = SlotIndex;
		TDPlayerController->GetTDHUD()->GetInventoryMenuWidget()->SetVisibility(ESlateVisibility::Visible);

	}

}

bool UTDUW_InventorySlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}
