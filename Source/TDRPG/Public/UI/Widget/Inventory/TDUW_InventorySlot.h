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
	void NativeConstruct() override;
	void UpdateInventorySlotUI(const FItem& InItem, int32 InItemQuantity);

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UImage> Image_Slot;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UImage> Image_Item;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> Text_ItemQuantity;

private:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
};
