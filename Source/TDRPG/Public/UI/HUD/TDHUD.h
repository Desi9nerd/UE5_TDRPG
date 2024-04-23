#pragma once
#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TDHUD.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;
class UTDWidgetControllerOverlay;
class UTDWidgetControllerAttributeMenu;
class UTDWidgetControllerSkillMenu;
class UTDUW;
struct FWidgetControllerParams;

/**
 * 
 */
UCLASS()
class TDRPG_API ATDHUD : public AHUD
{
	GENERATED_BODY()

public:
	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);
	TObjectPtr<UTDWidgetControllerOverlay> GetTDWidgetControllerOverlay(const FWidgetControllerParams& WCParams);
	TObjectPtr<UTDWidgetControllerAttributeMenu> GetTDWidgetControllerAttributeMenu(const FWidgetControllerParams& WCParams);
	TObjectPtr<UTDWidgetControllerSkillMenu> GetTDWidgetControllerSkillMenu(const FWidgetControllerParams& WCParams);

private:
	UPROPERTY()
	TObjectPtr<UTDUW>  OverlayWidget;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UTDUW> OverlayWidgetClass;

	UPROPERTY()
	TObjectPtr<UTDWidgetControllerOverlay> TDWidgetController_Overlay;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UTDWidgetControllerOverlay> TDWidgetController_OverlayClass;
	
	UPROPERTY()
	TObjectPtr<UTDWidgetControllerAttributeMenu> TDWidgetController_AttributeMenu;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UTDWidgetControllerAttributeMenu> TDWidgetController_AttributeMenuClass;

	UPROPERTY()
	TObjectPtr<UTDWidgetControllerSkillMenu> TDWidgetController_SkillMenu;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UTDWidgetControllerSkillMenu> TDWidgetController_SkillMenuClass;
};
