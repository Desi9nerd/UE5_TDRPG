#pragma once
#include "CoreMinimal.h"
#include "UI/Widget/TDUW.h"
#include "TDUW_InventoryCategory.generated.h"

enum class EItemCategory;
class UButton;
class UImage;

/**
 * 
 */

UCLASS()
class TDRPG_API UTDUW_InventoryCategory : public UTDUW
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Category;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_Category;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory Category")
	EItemCategory Category;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory Category")
	TObjectPtr<UTexture2D> CategoryIcon;
};
