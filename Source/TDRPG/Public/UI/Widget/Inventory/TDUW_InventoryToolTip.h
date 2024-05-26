#pragma once
#include "CoreMinimal.h"
#include "TDUW_InventorySlot.h"
#include "Blueprint/UserWidget.h"
#include "TDUW_InventoryToolTip.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class TDRPG_API UTDUW_InventoryToolTip : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UPROPERTY(VisibleAnywhere)
	UTDUW_InventorySlot* InventorySlotBeingHovered;

	UPROPERTY(meta = (BindWidgetOptional), meta = (AllowPrivateAccess))
	TObjectPtr<UTextBlock> ItemName;

	UPROPERTY(meta = (BindWidgetOptional), meta = (AllowPrivateAccess))
	TObjectPtr<UTextBlock> ItemCategoryType;

	UPROPERTY(meta = (BindWidgetOptional), meta = (AllowPrivateAccess))
	TObjectPtr<UTextBlock> ItemQuantity;

	UPROPERTY(meta = (BindWidgetOptional), meta = (AllowPrivateAccess))
	TObjectPtr<UTextBlock> ItemDescription;
};
