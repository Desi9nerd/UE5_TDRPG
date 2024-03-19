#pragma once
#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TDHUD.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;
class UTDWidgetControllerOverlay;
class UTDUserWidget;
struct FWidgetControllerParams;

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

	UTDWidgetControllerOverlay* GetWidgetControllerOverlay(const FWidgetControllerParams& WCParams);

	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UTDUserWidget> OverlayWidgetClass;

	UPROPERTY()
	TObjectPtr<UTDWidgetControllerOverlay> OverlayWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UTDWidgetControllerOverlay> OverlayWidgetControllerClass;
};
