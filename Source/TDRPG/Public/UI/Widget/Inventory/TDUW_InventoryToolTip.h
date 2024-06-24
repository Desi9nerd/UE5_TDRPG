#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TDUW_InventoryToolTip.generated.h"

class UTextBlock;
class UTDUW_InventoryEntry;

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
	TObjectPtr<UTDUW_InventoryEntry> InventorySlotBeingHovered;

	UPROPERTY(meta = (BindWidgetOptional), meta = (AllowPrivateAccess))
	TObjectPtr<UTextBlock> ItemName;

	UPROPERTY(meta = (BindWidgetOptional), meta = (AllowPrivateAccess))
	TObjectPtr<UTextBlock> ItemCategoryType;

	UPROPERTY(meta = (BindWidgetOptional), meta = (AllowPrivateAccess))
	TObjectPtr<UTextBlock> ItemQuantity;

	UPROPERTY(meta = (BindWidgetOptional), meta = (AllowPrivateAccess))
	TObjectPtr<UTextBlock> ItemDescription;
};
