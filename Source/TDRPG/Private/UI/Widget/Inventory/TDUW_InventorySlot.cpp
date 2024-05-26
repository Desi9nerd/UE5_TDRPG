#include "UI/Widget/Inventory/TDUW_InventorySlot.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Character/TDCharacter.h"
#include "Component/TDInventoryComponent.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetTextLibrary.h"
#include "Player/TDPlayerController.h"
#include "UI/DragDropOperation/TDDragDropOperation.h"
#include "UI/HUD/TDHUD.h"
#include "UI/Widget/Inventory/TDUW_Inventory.h"
#include "UI/Widget/Inventory/TDUW_InventoryMenuUI.h"
#include "UI/Widget/Inventory/TDUW_InventoryToolTip.h"

void UTDUW_InventorySlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (IsValid(ToolTipClass))
	{
		UTDUW_InventoryToolTip* ToolTip = CreateWidget<UTDUW_InventoryToolTip>(this, ToolTipClass);
		ToolTip->InventorySlotBeingHovered = this;
		SetToolTip(ToolTip);
	}
}

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
	ItemReference = InItem;

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

FReply UTDUW_InventorySlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		UE_LOG(LogTemp, Warning, TEXT("Native On Mouse Button Down!! "));

		// FReply::Handled()를 반환하여 이벤트 처리를 했음을 시스템에 알림.
		return Reply.Handled().DetectDrag(TakeWidget(), EKeys::LeftMouseButton);
	}

	return Reply.Unhandled();
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
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	UE_LOG(LogTemp, Warning, TEXT("Hello! OnDrop Test at TDUW_InventorySlot!!"));

	bool bZeroNstackable = GetNewSlot().ItemQuantity == 0 && GetDraggedSlot().Item.bStackable;
	bool bSameNameNstackable = GetNewSlot().Item.Name.EqualTo(GetDraggedSlot().Item.Name) && GetNewSlot().Item.bStackable && GetDraggedSlot().Item.bStackable;

	if (bZeroNstackable || bSameNameNstackable)
	{
		bIsStackable = true;
	}
	else
	{
		bIsStackable = false;
	}

	UpdateInventorySlots();

	ATDPlayerController* TDPlayerController = Cast<ATDPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	TDPlayerController->GetTDHUD()->GetInventoryMenuWidget()->SetVisibility(ESlateVisibility::HitTestInvisible);

	return true;
}

FInventorySlot& UTDUW_InventorySlot::GetDraggedSlot() const
{
	//****************************************************************************
	ATDPlayerController* TDPlayerController = Cast<ATDPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	ATDCharacter* TDCharacter = Cast<ATDCharacter>(TDPlayerController->GetCharacter());

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


	return (*SelectedCategoryItems)[TDPlayerController->GetTDHUD()->GetInventoryWidget()->DraggedSlotIndex];
}

FInventorySlot& UTDUW_InventorySlot::GetNewSlot() const
{
	//****************************************************************************
	ATDPlayerController* TDPlayerController = Cast<ATDPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	ATDCharacter* TDCharacter = Cast<ATDCharacter>(TDPlayerController->GetCharacter());

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


	return (*SelectedCategoryItems)[SlotIndex];
}

void UTDUW_InventorySlot::UpdateDraggedSlot(const FItem& DraggedSlotItem, int32 NewItemQuantity)
{
	//****************************************************************************
	ATDPlayerController* TDPlayerController = Cast<ATDPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	ATDCharacter* TDCharacter = Cast<ATDCharacter>(TDPlayerController->GetCharacter());

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


	int32 DraggedSlotIdx = TDPlayerController->GetTDHUD()->GetInventoryWidget()->DraggedSlotIndex;

	// Slot 업데이트
	(*SelectedCategoryItems)[DraggedSlotIdx].Item = DraggedSlotItem;
	(*SelectedCategoryItems)[DraggedSlotIdx].ItemQuantity = NewItemQuantity;

	(*SelectedCategoryItems)[DraggedSlotIdx].InventorySlot->UpdateInventorySlotUI(DraggedSlotItem, NewItemQuantity);
}

void UTDUW_InventorySlot::UpdateNewSlot(const FItem& DraggedSlotItem, int32 NewItemQuantity)
{
	//****************************************************************************
	ATDPlayerController* TDPlayerController = Cast<ATDPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	ATDCharacter* TDCharacter = Cast<ATDCharacter>(TDPlayerController->GetCharacter());

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
	

	// Slot 업데이트
	(*SelectedCategoryItems)[SlotIndex].Item = DraggedSlotItem;
	(*SelectedCategoryItems)[SlotIndex].ItemQuantity = NewItemQuantity;

	(*SelectedCategoryItems)[SlotIndex].InventorySlot->UpdateInventorySlotUI(DraggedSlotItem, NewItemQuantity);
}

void UTDUW_InventorySlot::UpdateInventorySlots()
{
	if (GetNewSlot().SlotIndex == GetDraggedSlot().SlotIndex) return;


	int32 L_NewSlotItemQuantity;
	if (bIsStackable) // 쌓는게 가능한 아이템이라면
	{
		L_NewSlotItemQuantity = GetNewSlot().ItemQuantity + GetDraggedSlot().ItemQuantity;

		if (L_NewSlotItemQuantity > GetDraggedSlot().Item.MaxStackSize) // MaxStackSize보다 큰 경우
		{
			UpdateNewSlot(GetDraggedSlot().Item, GetDraggedSlot().Item.MaxStackSize);
			UpdateDraggedSlot(GetDraggedSlot().Item, L_NewSlotItemQuantity - GetDraggedSlot().Item.MaxStackSize);
		}
		else
		{
			UpdateNewSlot(GetDraggedSlot().Item, L_NewSlotItemQuantity);
			UpdateDraggedSlot(FItem(), 0);
		}
	}
	else
	{
		FItem L_NewSlotItem = GetNewSlot().Item;
		L_NewSlotItemQuantity = GetNewSlot().ItemQuantity;

		UpdateNewSlot(GetDraggedSlot().Item, GetDraggedSlot().ItemQuantity);
		UpdateDraggedSlot(L_NewSlotItem, L_NewSlotItemQuantity);
	}
}
