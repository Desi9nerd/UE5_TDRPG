#pragma once
#include "CoreMinimal.h"
#include "UI/WidgetController/TDWidgetController.h"
#include "TDWidgetControllerSkillMenu.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class TDRPG_API UTDWidgetControllerSkillMenu : public UTDWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable)
	FOnPlayerStatChangedSignature SkillPointsChangedDelegate;

private:
	void AbilityChanged(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag);
	void SkillPointsChanged(int32 SkillPoints);

};