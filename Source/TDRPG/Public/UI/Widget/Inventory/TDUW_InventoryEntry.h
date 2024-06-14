#pragma once
#include "CoreMinimal.h"
#include "Library/TDItemLibrary.h"
#include "UI/Widget/TDUW.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "TDUW_InventoryEntry.generated.h"

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
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

	void UpdateInventorySlotUI(const FItem& InItem, int32 InItemQuantity);

	UPROPERTY()
	int32 SlotIndex;


	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_Slot;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_Item;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_ItemQuantity;
	
};
