#pragma once
#include "CoreMinimal.h"
#include "Library/TDItemLibrary.h"
#include "UI/Widget/TDUW.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "TDUW_InventoryEntry.generated.h"

class UGameplayEffect;
class UTDInventoryComponent;
class UImage;
class UTextBlock;
class UTDUW_InventoryToolTip;
/**
 * 
 */
UCLASS()
class TDRPG_API UTDUW_InventoryEntry : public UTDUW, public IUserObjectListEntry
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FItem ItemReference;

	FORCEINLINE FItem GetFItemReference() const { return ItemReference; }
	TObjectPtr<UTDInventoryComponent> GetTDInventoryComponent();
	TArray<UTDInventoryDisplayItemObject*> GetTDInventoryDisplayItemObjects();

	virtual void NativeOnInitialized() override;
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

	void UpdateInventorySlotUI(const FItem& InItem, int32 InItemQuantity);


	void SetSlotIndex(int32 InSlotIndex); // SlotIndex 설정 함수 추가
	UPROPERTY()
	int32 SlotIndex;


	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_Slot;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_Item;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_ItemQuantity;

protected:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

private:
	void UpdateInventorySlots();

	FInventoryDisplayItem& GetDraggedSlot();
	FInventoryDisplayItem& GetNewSlot();
	void UpdateDraggedSlot(const FItem& DraggedSlotItem, int32 NewItemQuantity);
	void UpdateNewSlot(const FItem& DraggedSlotItem, int32 NewItemQuantity);

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess))
	TSubclassOf<UTDUW_InventoryToolTip> ToolTipClass;
};
