#pragma once
#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/Widget/TDUserWidget.h"
#include "TDHUD.generated.h"

/**
 * 
 */
UCLASS()
class TDRPG_API ATDHUD : public AHUD
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TObjectPtr<UTDUserWidget>  OverlayWidget;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UTDUserWidget> OverlayWidgetClass;
};
