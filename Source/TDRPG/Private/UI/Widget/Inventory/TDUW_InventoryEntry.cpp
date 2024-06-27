#include "UI/Widget/Inventory/TDUW_InventoryEntry.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "Actor/TDEffectActor.h"
#include "Actor/TDItemActor.h"
#include "Character/TDCharacter.h"
#include "Component/TDInventoryComponent.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Player/TDPlayerController.h"
#include "UI/DragDropOperation/TDDragDropOperation.h"
#include "UI/HUD/TDHUD.h"
#include "UI/Widget/Inventory/TDUW_InventoryPanel.h"
#include "UI/Widget/Inventory/TDUW_InventoryToolTip.h"

TObjectPtr<UTDInventoryComponent> UTDUW_InventoryEntry::GetTDInventoryComponent()
{
	ATDPlayerController* TDPlayerController = Cast<ATDPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	ATDCharacter* TDCharacter = Cast<ATDCharacter>(TDPlayerController->GetCharacter());

	return TDCharacter->GetInventoryComponent();
}

TArray<UTDInventoryDisplayItemObject*> UTDUW_InventoryEntry::GetTDInventoryDisplayItemObjects()
{
	ATDPlayerController* TDPlayerController = Cast<ATDPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	ATDCharacter* TDCharacter = Cast<ATDCharacter>(TDPlayerController->GetCharacter());

	return TDCharacter->GetInventoryComponent()->InventoryDisplayItems;
}

void UTDUW_InventoryEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (IsValid(ToolTipClass))
	{
		UTDUW_InventoryToolTip* ToolTip = CreateWidget<UTDUW_InventoryToolTip>(this, ToolTipClass);
		ToolTip->InventorySlotBeingHovered = this;
		SetToolTip(ToolTip);
	}
}

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

	// 여기서 SlotIndex를 설정합니다.
	SetSlotIndex(Item->Data.SlotIndex);
	UE_LOG(LogTemp, Warning, TEXT("Slot Index = %d"), SlotIndex);

		
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

void UTDUW_InventoryEntry::SetSlotIndex(int32 InSlotIndex)
{
	SlotIndex = InSlotIndex;
}

FReply UTDUW_InventoryEntry::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		UE_LOG(LogTemp, Warning, TEXT("Native On Mouse Button Down!! "));

		// FReply::Handled()를 반환하여 이벤트 처리를 했음을 시스템에 알림.
		return Reply.Handled().DetectDrag(TakeWidget(), EKeys::LeftMouseButton);
	}
	else if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		UE_LOG(LogTemp, Warning, TEXT("Native On Mouse Button Down - Right Click "));
		UE_LOG(LogTemp, Log, TEXT("%s"), *GetTDInventoryComponent()->InventoryDisplayItems[SlotIndex]->Data.Item.Name.ToString());
		UE_LOG(LogTemp, Warning, TEXT("Slot Index = %d"), SlotIndex);

		//** 아이템 사용하기
		if (GetTDInventoryComponent()->InventoryDisplayItems[SlotIndex]->Data.Item.bConsumable &&
			GetTDInventoryComponent()->InventoryDisplayItems[SlotIndex]->Data.ItemQuantity > 0)
		{
			FText ItemName = GetTDInventoryComponent()->InventoryDisplayItems[SlotIndex]->Data.Item.Name;
			GetTDInventoryComponent()->ConsumeItem(ItemName);
			GetTDInventoryComponent()->InventoryDisplayItems[SlotIndex]->Data.ItemQuantity -= 1;
		}

	}

	return Reply.Unhandled();
}

void UTDUW_InventoryEntry::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	UE_LOG(LogTemp, Warning, TEXT("Inventory Slot NativeOn Drag Detected!! "));
	if (IsValid(OutOperation)) return;


	if (GetTDInventoryComponent()->InventoryDisplayItems[SlotIndex]->Data.ItemQuantity != 0)
	{
		UTDDragDropOperation* DragItemOperation = NewObject<UTDDragDropOperation>();
		DragItemOperation->DefaultDragVisual = this;
		DragItemOperation->Payload = this;

		OutOperation = DragItemOperation;

		ATDPlayerController* TDPlayerController = Cast<ATDPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
		TDPlayerController->GetTDHUD()->GetInventoryPanelWidget()->DraggedSlotIndex = SlotIndex;
	}

}

bool UTDUW_InventoryEntry::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,	UDragDropOperation* InOperation)
{
	UE_LOG(LogTemp, Warning, TEXT("Native On Drop Test. UTDUW_InventoryEntry::NativeOnDrop"));

	UTDDragDropOperation* DragOperation = Cast<UTDDragDropOperation>(InOperation);
	if (false == IsValid(DragOperation)) return false;
		
	UpdateInventorySlots();

	ATDPlayerController* TDPlayerController = Cast<ATDPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	TDPlayerController->GetTDHUD()->GetInventoryPanelWidget()->SetVisibility(ESlateVisibility::HitTestInvisible);

	return true;
}

void UTDUW_InventoryEntry::UpdateInventorySlots()
{
	if (GetNewSlot().SlotIndex == GetDraggedSlot().SlotIndex) return;

	FItem PrevSlotNetItem = GetNewSlot().Item;
	int32 PrevSlotNewQuantity = GetNewSlot().ItemQuantity;

	UpdateNewSlot(GetDraggedSlot().Item, GetDraggedSlot().ItemQuantity);
	UpdateDraggedSlot(PrevSlotNetItem, PrevSlotNewQuantity);
}

FInventoryDisplayItem& UTDUW_InventoryEntry::GetDraggedSlot()
{
	ATDPlayerController* TDPlayerController = Cast<ATDPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

	return GetTDInventoryComponent()->InventoryDisplayItems[TDPlayerController->GetTDHUD()->GetInventoryPanelWidget()->DraggedSlotIndex]->Data;
}

FInventoryDisplayItem& UTDUW_InventoryEntry::GetNewSlot()
{
	return GetTDInventoryComponent()->InventoryDisplayItems[SlotIndex]->Data;
}

void UTDUW_InventoryEntry::UpdateDraggedSlot(const FItem& DraggedSlotItem, int32 NewItemQuantity)
{
	ATDPlayerController* TDPlayerController = Cast<ATDPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	int32 DraggedSlotIdx = TDPlayerController->GetTDHUD()->GetInventoryPanelWidget()->DraggedSlotIndex;

	// Slot 업데이트
	GetTDInventoryComponent()->InventoryDisplayItems[DraggedSlotIdx]->Data.Item = DraggedSlotItem;
	GetTDInventoryComponent()->InventoryDisplayItems[DraggedSlotIdx]->Data.ItemQuantity = NewItemQuantity;

	TDPlayerController->GetTDHUD()->GetInventoryPanelWidget()->DisplayInventorySlotWidgets();
}

void UTDUW_InventoryEntry::UpdateNewSlot(const FItem& DraggedSlotItem, int32 NewItemQuantity)
{
	ATDPlayerController* TDPlayerController = Cast<ATDPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

	GetTDInventoryComponent()->InventoryDisplayItems[SlotIndex]->Data.Item = DraggedSlotItem;
	GetTDInventoryComponent()->InventoryDisplayItems[SlotIndex]->Data.ItemQuantity = NewItemQuantity;

	TDPlayerController->GetTDHUD()->GetInventoryPanelWidget()->DisplayInventorySlotWidgets();
}
