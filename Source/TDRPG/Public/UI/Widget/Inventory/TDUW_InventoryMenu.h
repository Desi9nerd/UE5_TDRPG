#pragma once
#include "CoreMinimal.h"
#include "UI/Widget/TDUW.h"
#include "TDUW_InventoryMenu.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class TDRPG_API UTDUW_InventoryMenu : public UTDUW
{
	GENERATED_BODY()

private:
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	UPROPERTY(meta = (BindWidgetOptional), meta = (AllowPrivateAccess))
	TObjectPtr<UButton> Button_InventoryMenu;

	UPROPERTY(meta = (BindWidgetOptional), meta = (AllowPrivateAccess))
	TObjectPtr<UImage> Image_InventoryMenu;


	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess))
	TObjectPtr<USoundBase> Sound_DropItem;
};
