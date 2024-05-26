#pragma once
#include "CoreMinimal.h"
#include "Library/TDItemLibrary.h"
#include "UI/Widget/TDUW.h"
#include "TDUW_InventorySlot.generated.h"

class UImage;
class UTextBlock;
/**
 * 
 */
UCLASS()
class TDRPG_API UTDUW_InventorySlot : public UTDUW
{
	GENERATED_BODY()

public:
	UPROPERTY()
	int32 SlotIndex;

	void NativeConstruct() override;
	void UpdateInventorySlotUI(const FItem& InItem, int32 InItemQuantity);

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UImage> Image_Slot;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UImage> Image_Item;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> Text_ItemQuantity;


	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

private:
	FInventorySlot& GetDraggedSlot() const;
	FInventorySlot& GetNewSlot() const;
	void UpdateDraggedSlot(const FItem& DraggedSlotItem, int32 NewItemQuantity);
	void UpdateNewSlot(const FItem& DraggedSlotItem, int32 NewItemQuantity);

	void UpdateInventorySlots();

	bool bIsStackable;
};
