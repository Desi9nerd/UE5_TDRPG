#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TDUserWidget.generated.h"

class UTDWidgetController;
class UTDWidgetControllerOverlay;
class UTDWidgetControllerAttributeMenu;

/**
 * 
 */
UCLASS()
class TDRPG_API UTDUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* InWidgetController);

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController;

	/////////////////////////////////////////////////
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTDWidgetController> TDWidgetController;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTDWidgetControllerOverlay> TDWidgetControllerOverlay;
	/////////////////////////////////////////////////

	//** TDAbilitySystemBPLibrary �Լ� �ű��
	UFUNCTION(Category = "WidgetController")
	static UTDWidgetControllerOverlay* GetWidgetControllerOverlay(const UObject* WorldContextObject);
	UFUNCTION(Category = "WidgetController")
	static UTDWidgetControllerAttributeMenu* GetAttributeMenuWidgetController(const UObject* WorldContextObject);

protected:
	UFUNCTION(BlueprintImplementableEvent) // BP���� ����
	void WidgetControllerSet();
};
