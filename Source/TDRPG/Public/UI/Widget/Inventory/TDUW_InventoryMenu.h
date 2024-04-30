#pragma once
#include "CoreMinimal.h"
#include "UI/Widget/TDUW.h"
#include "TDUW_InventoryMenu.generated.h"

class UVerticalBox;

/**
 * 
 */

UCLASS()
class TDRPG_API UTDUW_InventoryMenu : public UTDUW
{
	GENERATED_BODY()

public:
	virtual TSharedRef<SWidget> RebuildWidget() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox> VB_Categories;
};
