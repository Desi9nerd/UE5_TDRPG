#pragma once
#include "CoreMinimal.h"
#include "UI/WidgetController/TDWidgetController.h"
#include "GameplayTags/TDGameplayTags.h"
#include "TDWidgetControllerSkillMenu.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FSkillIconSelectedSignature, bool, bSkillPointsButtonEnabled, bool, bEquipButtonEnabled, FString, DescriptionString, FString, NextLevelDescriptionString);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWaitingForEquipSelectionSignature, const FGameplayTag&, AbilityType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSkillIconReassignedSignature, const FGameplayTag&, AbilityTag);

/** SkillMenu
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
	//*****************************************************
	//** Delegate 등록
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;
	//*****************************************************

	UFUNCTION(BlueprintCallable)
	void SelectSkillIcon(const FGameplayTag& AbilityTag);

	UFUNCTION(BlueprintCallable)
	void SkillPointButtonPressed();

	UFUNCTION(BlueprintCallable)
	void DeselectSkillIcon();

	UFUNCTION(BlueprintCallable)
	void EquipButtonPressed();

	UFUNCTION(BlueprintCallable)
	void SkillRowGlobePressed(const FGameplayTag& SlotTag, const FGameplayTag& AbilityType);

	UPROPERTY(BlueprintAssignable)
	FOnPlayerStatChangedSignature SkillPointsChangedDelegate;

	UPROPERTY(BlueprintAssignable)
	FSkillIconSelectedSignature SkillIconSelectedDelegate;

	UPROPERTY(BlueprintAssignable)
	FWaitingForEquipSelectionSignature WaitingForEquipDelegate;

	UPROPERTY(BlueprintAssignable)
	FWaitingForEquipSelectionSignature StopWaitingForEquipDelegate;

	UPROPERTY(BlueprintAssignable)
	FSkillIconReassignedSignature SkillIconReassignedDelegate;

private:
	static void UpdateButtons_bEnableToClick(const FGameplayTag& AbilityStatus, int32 SkillPoints, bool& bEnableSkillPointsButton, bool& bEnableEquipButton);

	void AbilityChanged(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 InAbilityLevel);
	void SkillPointsChanged(int32 SkillPoints);

	void OnEquippedAbility(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, const FGameplayTag& SlotTag, const FGameplayTag& PreviousSlotTag);

	FSelectedSkillInSkillMenu SelectedAbilityInSkillMenu = { FTDGameplayTags::GetTDGameplayTags().Abilities_None,  FTDGameplayTags::GetTDGameplayTags().Abilities_Status_Locked };
	int32 CurrentSkillPoints = 0;
	bool bWaitingForEquipSelection = false;

	FGameplayTag SelectedSlot; // 장착버튼을 누른 후 어떤slot을 선택했는지 기록하기 위해 InputTag를 담음.
};