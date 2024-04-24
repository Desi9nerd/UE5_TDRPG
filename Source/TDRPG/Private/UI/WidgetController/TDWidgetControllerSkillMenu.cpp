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


	//** GameplayTag �����ϱ�
	const FTDGameplayTags TDGameplayTags = FTDGameplayTags::GetTDGameplayTags();

	FGameplayTag AbilityStatusGameplayTag;
	const FGameplayAbilitySpec* TempAbilitySpec = GetTDASC()->GetSpecFromAbilityTag(AbilityTag);

	if (false == AbilityTag.IsValid() || AbilityTag.MatchesTag(TDGameplayTags.Abilities_None) || 
		nullptr == TempAbilitySpec)
	{
		AbilityStatusGameplayTag = TDGameplayTags.Abilities_Status_Locked;
	}
	else
	{
		AbilityStatusGameplayTag = GetTDASC()->GetStatusFromSpec(*TempAbilitySpec);
	}


	//** ��ųƮ�� �� ��ų����Ʈ ��ư�� ���� ��ư Ȱ��ȭ ���� �����ϱ�
	SelectedSkillInSkillMenu.Ability = AbilityTag;
	SelectedSkillInSkillMenu.Status = AbilityStatusGameplayTag;
	bool bEnableSkillPointsButton = false;
	bool bEnableEquipButton = false;

	// bool���� bEnableSkillPoints, bEnableEquip�� ���ݷ���(&)�� �Ѱ� ������Ʈ�Ѵ�.
	UpdateButtons_bEnableToClick(AbilityStatusGameplayTag, SkillPoints, bEnableSkillPointsButton, bEnableEquipButton);

	// ������Ʈ�� bEnableSkillPoints, bEnableEquip�� Broadcast�ؼ� �˸���.
	SkillIconSelectedDelegate.Broadcast(bEnableSkillPointsButton, bEnableEquipButton);
}

// ��ųƮ�� ���� ���� ��ųȹ�� ��ư, ���� ��ư
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
	/* ���� ���ǵ鿡 �ش����� �ʴ´ٸ� TDGameplayTags.Abilities_Status_Locked ����. ��ųȹ�� ��ư, ���� ��ư ��� ��Ȱ��ȭ */
}

void UTDWidgetControllerSkillMenu::AbilityChanged(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag)
{
	//** ����
	//   ��ų�� StatusTag�� Braodcast�� �˸���.
	if (SelectedSkillInSkillMenu.Ability.MatchesTagExact(AbilityTag))
	{
		SelectedSkillInSkillMenu.Status = StatusTag;
		bool bEnableSpendPoints = false;
		bool bEnableEquip = false;

		UpdateButtons_bEnableToClick(StatusTag, CurrentSkillPoints, bEnableSpendPoints, bEnableEquip);
		SkillIconSelectedDelegate.Broadcast(bEnableSpendPoints, bEnableEquip);
	}

	//** Data Asset
	//   ��ų�� StatusTag�� Braodcast�� �˸���.
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