#pragma once
#include "CoreMinimal.h"
#include "UI/Widget/Inventory/TDUW_Inventory.h"
#include "TDUW_InventorySlot.generated.h"

/**
 * 
 */
UCLASS()
class TDRPG_API UTDUW_InventorySlot : public UTDUW_Inventory
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UImage> Image_Slot;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UImage> Image_Item;
};
