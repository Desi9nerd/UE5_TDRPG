#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GAS/Data/TDDA_Ability.h"
#include "TDUW.generated.h"

class UImage;
class USizeBox;
class UTDWidgetController;
class UTDWidgetControllerOverlay;
class UTDWidgetControllerAttributeMenu;
class UTDWidgetControllerSkillMenu;
class UAbilitySystemComponent;

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTDWidgetControllerSkillMenu> TDWidgetControllerSkillMenu;
	/////////////////////////////////////////////////

	//** TDAbilitySystemBPLibrary 함수 옮기기
	UFUNCTION(BlueprintCallable, Category = "WidgetController")
	UTDWidgetControllerOverlay* GetWidgetControllerOverlay(const UObject* WorldContextObject);
	UFUNCTION(BlueprintCallable, Category = "WidgetController")
	UTDWidgetControllerAttributeMenu* GetAttributeMenuWidgetController(const UObject* WorldContextObject);
	UFUNCTION(BlueprintCallable, Category = "WidgetController")
	UTDWidgetControllerSkillMenu* GetWidgetControllerSkillMenu(const UObject* WorldContextObject);

protected:
	UFUNCTION(BlueprintImplementableEvent) // BP에서 실행
	void WidgetControllerSet();

	//********************************************************
	void UpdateBoxSize(USizeBox* OutSizeBox, int InWidth, int InHeight);
	void UpdateBrush(UImage* OutImage, FSlateBrush InBrush);
	void SetPadding(UImage* OutImage, float InLeft, float InRight, float InTop, float InBottom);
	//********************************************************


private:
	UFUNCTION()
	void OnAbilitySystemComponentInitialized();
};
