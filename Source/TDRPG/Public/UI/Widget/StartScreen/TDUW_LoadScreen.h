#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TDUW_LoadScreen.generated.h"

/**
 * 
 */

UCLASS()
class TDRPG_API UTDUW_LoadScreen : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void InitializeWidgetBP();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SlotIndex;

};
