#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TDUW.generated.h"

class UTDWidgetController;
class UTDWidgetControllerOverlay;
class UTDWidgetControllerAttributeMenu;

/**
 * 
 */
UCLASS()
class TDRPG_API UTDUW : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	virtual void SetWidgetController(UObject* InWidgetController);

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController;

	/////////////////////////////////////////////////
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTDWidgetController> TDWidgetController;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTDWidgetControllerOverlay> TDWidgetControllerOverlay;
	/////////////////////////////////////////////////

	//** TDAbilitySystemBPLibrary 함수 옮기기
	UFUNCTION(Category = "WidgetController")
	static UTDWidgetControllerOverlay* GetWidgetControllerOverlay(const UObject* WorldContextObject);
	UFUNCTION(Category = "WidgetController")
	static UTDWidgetControllerAttributeMenu* GetAttributeMenuWidgetController(const UObject* WorldContextObject);

protected:
	UFUNCTION(BlueprintImplementableEvent) // BP에서 실행
	void WidgetControllerSet();
};
