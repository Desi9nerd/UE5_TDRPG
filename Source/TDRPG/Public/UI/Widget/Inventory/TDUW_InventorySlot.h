#pragma once
#include "CoreMinimal.h"
#include "Library/TDItemLibrary.h"
#include "UI/Widget/Inventory/TDUW_Inventory.h"
#include "TDUW_InventorySlot.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class TDRPG_API UTDUW_InventorySlot : public UTDUW_Inventory
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;
	void UpdateInventorySlotUI(FItem InItem, int32 InItemQuantity);

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UImage> Image_Slot;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UImage> Image_Item;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> Text_ItemQuantity;
};
