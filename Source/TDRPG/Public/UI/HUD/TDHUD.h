#pragma once
#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TDHUD.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;
class UTDWidgetControllerOverlay;
class UTDWidgetControllerAttributeMenu;
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

	UTDWidgetControllerOverlay* GetWidgetControllerOverlay(const FWidgetControllerParams& WCParams);
	UTDWidgetControllerAttributeMenu* GetWidgetControllerAttributeMenu(const FWidgetControllerParams& WCParams);

	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);
	//void InitAttributeMenu(APlayerController* PC, APlayeraState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);

private:
	UPROPERTY()
	TObjectPtr<UTDUserWidget>  OverlayWidget;
	/*UPROPERTY()
	TObjectPtr<UTDUserWidget>  AttributeMenuWidget;*/


	UPROPERTY(EditAnywhere)
	TSubclassOf<UTDUserWidget> OverlayWidgetClass;
	UPROPERTY()
	TObjectPtr<UTDWidgetControllerOverlay> OverlayWidgetController;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UTDWidgetControllerOverlay> OverlayWidgetControllerClass;

	//UPROPERTY(EditAnywhere)
	//TSubclassOf<UTDUserWidget> AttributeMenuWidgetClass;
	UPROPERTY()
	TObjectPtr<UTDWidgetControllerAttributeMenu> AttributeMenuWidgetController;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UTDWidgetControllerAttributeMenu> AttributeMenuWidgetControllerClass;
};
