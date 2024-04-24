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

void UTDWidgetControllerSkillMenu::SkillIconSelected(const FGameplayTag& AbilityTag)
{
	//** SkillPoints
	const int32 SkillPoints = GetTDPlayerState()->GetSkillPoints();


	//** GameplayTag 설정하기
	const FTDGameplayTags TDGameplayTags = FTDGameplayTags::GetTDGameplayTags();

	FGameplayTag AbilityStatusTag;
	const FGameplayAbilitySpec* TempAbilitySpec = GetTDASC()->GetSpecFromAbilityTag(AbilityTag);

	if (false == AbilityTag.IsValid() || AbilityTag.MatchesTag(TDGameplayTags.Abilities_None) || 
		nullptr == TempAbilitySpec)
	{
		AbilityStatusTag = TDGameplayTags.Abilities_Status_Locked;
	}
	else
	{
		AbilityStatusTag = GetTDASC()->GetStatusFromSpec(*TempAbilitySpec);
	}


	//** 스킬트리 내 스킬포인트 버튼과 장착 버튼 활성화 여부 설정하기
	SelectedSkillInSkillMenu.Ability = AbilityTag;
	SelectedSkillInSkillMenu.Status = AbilityStatusTag;
	bool bEnableSkillPointsButton = false;
	bool bEnableEquipButton = false;

	// bool변수 bEnableSkillPoints, bEnableEquip를 레펀런스(&)로 넘겨 업데이트한다.
	UpdateButtons_bEnableToClick(AbilityStatusTag, SkillPoints, bEnableSkillPointsButton, bEnableEquipButton);

	// 업데이트된 bEnableSkillPoints, bEnableEquip를 Broadcast해서 알린다.
	SkillIconSelectedDelegate.Broadcast(bEnableSkillPointsButton, bEnableEquipButton);
}

void UTDWidgetControllerSkillMenu::SkillPointButtonPressed()
{
	if (GetTDASC())
	{
		GetTDASC()->ServerSpendSkillPoint(SelectedSkillInSkillMenu.Ability); // Server RPC
	}
}

// 스킬트리 위젯 내의 스킬획득 버튼, 장착 버튼
void UTDWidgetControllerSkillMenu::UpdateButtons_bEnableToClick(const FGameplayTag& AbilityStatus, int32 SkillPoints, bool& bEnableSkillPointsButton, bool& bEnableEquipButton)
{
	const FTDGameplayTags TDGameplayTags = FTDGameplayTags::GetTDGameplayTags();

	bEnableSkillPointsButton = false;
	bEnableEquipButton = false;
	if (AbilityStatus.MatchesTagExact(TDGameplayTags.Abilities_Status_Equipped))
	{
		bEnableEquipButton = true;
		if (SkillPoints > 0)
		{
			bEnableSkillPointsButton = true;
		}
	}
	else if (AbilityStatus.MatchesTagExact(TDGameplayTags.Abilities_Status_Eligible))
	{
		if (SkillPoints > 0)
		{
			bEnableSkillPointsButton = true;
		}
	}
	else if (AbilityStatus.MatchesTagExact(TDGameplayTags.Abilities_Status_Unlocked))
	{
		bEnableEquipButton = true;
		if (SkillPoints > 0)
		{
			bEnableSkillPointsButton = true;
		}
	}
	/* 위의 조건들에 해당하지 않는다면 TDGameplayTags.Abilities_Status_Locked 상태. 스킬획득 버튼, 장착 버튼 모두 비활성화 */
}

void UTDWidgetControllerSkillMenu::AbilityChanged(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 InAbilityLevel)
{
	//** 위젯
	//   스킬의 StatusTag를 Braodcast로 알린다.
	if (SelectedSkillInSkillMenu.Ability.MatchesTagExact(AbilityTag))
	{
		SelectedSkillInSkillMenu.Status = StatusTag;
		bool bEnableSpendPoints = false;
		bool bEnableEquip = false;

		UpdateButtons_bEnableToClick(StatusTag, CurrentSkillPoints, bEnableSpendPoints, bEnableEquip);
		SkillIconSelectedDelegate.Broadcast(bEnableSpendPoints, bEnableEquip);
	}

	//** Data Asset
	//   스킬의 StatusTag를 Braodcast로 알린다.
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


	CurrentSkillPoints = SkillPoints;

	bool bEnableSpendPoints = false;
	bool bEnableEquip = false;

	UpdateButtons_bEnableToClick(SelectedSkillInSkillMenu.Status, CurrentSkillPoints, bEnableSpendPoints, bEnableEquip);
	SkillIconSelectedDelegate.Broadcast(bEnableSpendPoints, bEnableEquip);
}