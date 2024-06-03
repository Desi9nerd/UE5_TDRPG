#pragma once
#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TDHUD.generated.h"

class UTDUW_InventoryPanel;
class UAttributeSet;
class UAbilitySystemComponent;
class UTDWidgetControllerOverlay;
class UTDWidgetControllerAttributeMenu;
class UTDWidgetControllerSkillMenu;
class UTDUW;
class UTDUW_Inventory;
class UTDUW_InventoryMenuUI;
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
	//void InitInventory(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);

	TObjectPtr<UTDUW_Inventory> GetInventoryWidget();
	TObjectPtr<UTDUW_InventoryPanel> GetInventoryPanelWidget();
	TObjectPtr<UTDUW_InventoryMenuUI> GetInventoryMenuWidget();
	TObjectPtr<UTDWidgetControllerOverlay> GetTDWidgetControllerOverlay(const FWidgetControllerParams& WCParams);
	TObjectPtr<UTDWidgetControllerAttributeMenu> GetTDWidgetControllerAttributeMenu(const FWidgetControllerParams& WCParams);
	TObjectPtr<UTDWidgetControllerSkillMenu> GetTDWidgetControllerSkillMenu(const FWidgetControllerParams& WCParams);

private:
	UPROPERTY()
	TObjectPtr<UTDUW>  OverlayWidget;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UTDUW> OverlayWidgetClass;

	UPROPERTY()
	TObjectPtr<UTDUW_Inventory>  InventoryWidget;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UTDUW_Inventory> InventoryWidgetClass;
	UPROPERTY()
	TObjectPtr<UTDUW_InventoryPanel>  InventoryPanelWidget;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UTDUW_InventoryPanel> InventoryPanelWidgetClass;
	UPROPERTY()
	TObjectPtr<UTDUW_InventoryMenuUI>  InventoryMenuWidget;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UTDUW_InventoryMenuUI> InventoryMenuWidgetClass;

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
