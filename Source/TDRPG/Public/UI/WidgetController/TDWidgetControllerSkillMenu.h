#pragma once
#include "CoreMinimal.h"
#include "UI/WidgetController/TDWidgetController.h"
#include "GameplayTags/TDGameplayTags.h"
#include "TDWidgetControllerSkillMenu.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FSkillIconSelectedSignature, bool, bSkillPointsButtonEnabled, bool, bEquipButtonEnabled, FString, DescriptionString, FString, NextLevelDescriptionString);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWaitingForEquipSelectionSignature, const FGameplayTag&, AbilityType);

/**
 * 
 */

struct FSelectedSkillInSkillMenu
{
	FGameplayTag Ability = FGameplayTag();
	FGameplayTag Status = FGameplayTag();
};

UCLASS(BlueprintType, Blueprintable)
class TDRPG_API UTDWidgetControllerSkillMenu : public UTDWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	UFUNCTION(BlueprintCallable)
	void SelectSkillIcon(const FGameplayTag& AbilityTag);

	UFUNCTION(BlueprintCallable)
	void SkillPointButtonPressed();

	UFUNCTION(BlueprintCallable)
	void DeselectSkillIcon();

	UFUNCTION(BlueprintCallable)
	void EquipButtonPressed();

	UPROPERTY(BlueprintAssignable)
	FOnPlayerStatChangedSignature SkillPointsChangedDelegate;

	UPROPERTY(BlueprintAssignable)
	FSkillIconSelectedSignature SkillIconSelectedDelegate;

	UPROPERTY(BlueprintAssignable)
	FWaitingForEquipSelectionSignature WaitingForEquipDelegate;

	UPROPERTY(BlueprintAssignable)
	FWaitingForEquipSelectionSignature StopWaitingForEquipDelegate;

private:
	static void UpdateButtons_bEnableToClick(const FGameplayTag& AbilityStatus, int32 SkillPoints, bool& bEnableSkillPointsButton, bool& bEnableEquipButton);

	void AbilityChanged(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 InAbilityLevel);
	void SkillPointsChanged(int32 SkillPoints);

	FSelectedSkillInSkillMenu SelectedAbilityInSkillMenu = { FTDGameplayTags::GetTDGameplayTags().Abilities_None,  FTDGameplayTags::GetTDGameplayTags().Abilities_Status_Locked };
	int32 CurrentSkillPoints = 0;
	bool bWaitingForEquipSelection = false;
	
};