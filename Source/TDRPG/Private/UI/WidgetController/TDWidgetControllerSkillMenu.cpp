#include "UI/WidgetController/TDWidgetControllerSkillMenu.h"
#include "GAS/TDAbilitySystemComponent.h"
#include "GAS/Data/TDDA_Ability.h"
#include "Player/TDPlayerState.h"

void UTDWidgetControllerSkillMenu::BroadcastInitialValues()
{
	BroadcastDA_AbilityInfo();

	SkillPointsChangedDelegate.Broadcast(GetTDPlayerState()->GetSkillPoints());
}

void UTDWidgetControllerSkillMenu::BindCallbacksToDependencies()
{
	GetTDASC()->AbilityStatusChangedDelegate.AddUObject(this, &ThisClass::AbilityChanged);
	GetTDPlayerState()->OnSkillPointsChangedDelegate.AddUObject(this, &ThisClass::SkillPointsChanged);
}

void UTDWidgetControllerSkillMenu::AbilityChanged(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag)
{
	if (TDDA_Ability)
	{
		FDA_Ability DA_AbilityInfo = TDDA_Ability->FindDA_AbilityForTag(AbilityTag);
		DA_AbilityInfo.StatusTag = StatusTag;

		DA_AbilityInfoDelegate.Broadcast(DA_AbilityInfo);
	}
}

void UTDWidgetControllerSkillMenu::SkillPointsChanged(int32 SkillPoints)
{
	SkillPointsChangedDelegate.Broadcast(SkillPoints);
}