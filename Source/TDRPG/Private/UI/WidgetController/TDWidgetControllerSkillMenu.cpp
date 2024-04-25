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

	// bool변수 bEnableSkillPointsButton, bEnableEquipButton를 레펀런스(&)로 넘겨 업데이트한다.
	UpdateButtons_bEnableToClick(AbilityStatusTag, SkillPoints, bEnableSkillPointsButton, bEnableEquipButton);

	// Description, NextLevelDescription를 레펀런스(&)로 넘겨 업데이트한다.
	FString Description;
	FString NextLevelDescription;
	GetTDASC()->GetDescriptionsByAbilityTag(SelectedSkillInSkillMenu.Ability, Description, NextLevelDescription);

	// 4개 Broadcast ( bEnableSkillPointsButton, bEnableEquipButton, Description, NextLevelDescription )
	SkillIconSelectedDelegate.Broadcast(bEnableSkillPointsButton, bEnableEquipButton, Description, NextLevelDescription);
}

void UTDWidgetControllerSkillMenu::SkillPointButtonPressed()
{
	if (GetTDASC())
	{
		GetTDASC()->ServerSpendSkillPoint(SelectedSkillInSkillMenu.Ability); // Server RPC
	}
}

void UTDWidgetControllerSkillMenu::DeselectSkillIcon()
{
	// 선택 해제한 스킬아이콘의 GameplayTag - Ability, Status 변경. 
	SelectedSkillInSkillMenu.Ability = FTDGameplayTags::GetTDGameplayTags().Abilities_None;
	SelectedSkillInSkillMenu.Status = FTDGameplayTags::GetTDGameplayTags().Abilities_Status_Locked;

	// 4개 Broadcast - 버튼 2개가 안 눌리도록, 스킬 설명이 빈 텍스트로 나오도록 알림.
	SkillIconSelectedDelegate.Broadcast(false, false, FString(), FString());
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
		// bEnableSkillPointsButton, bEnableEquipButton 업데이트
		SelectedSkillInSkillMenu.Status = StatusTag;
		bool bEnableSkillPointsButton = false;
		bool bEnableEquipButton = false;
		UpdateButtons_bEnableToClick(StatusTag, CurrentSkillPoints, bEnableSkillPointsButton, bEnableEquipButton);

		// Description, NextLevelDescription 업데이트
		FString Description;
		FString NextLevelDescription;
		GetTDASC()->GetDescriptionsByAbilityTag(AbilityTag, Description, NextLevelDescription);

		// 4개 Broadcast ( bEnableSkillPointsButton, bEnableEquipButton, Description, NextLevelDescription )
		SkillIconSelectedDelegate.Broadcast(bEnableSkillPointsButton, bEnableEquipButton, Description, NextLevelDescription);
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
	//* SkillPoints이 변경된것 Broadcast
	SkillPointsChangedDelegate.Broadcast(SkillPoints);

	// bEnableSpendPoints, bEnableEquipButton 업데이트
	CurrentSkillPoints = SkillPoints;
	bool bEnableSkillPointsButton = false;
	bool bEnableEquipButton = false;
	UpdateButtons_bEnableToClick(SelectedSkillInSkillMenu.Status, CurrentSkillPoints, bEnableSkillPointsButton, bEnableEquipButton);

	// Description, NextLevelDescription 업데이트
	FString Description;
	FString NextLevelDescription;
	GetTDASC()->GetDescriptionsByAbilityTag(SelectedSkillInSkillMenu.Ability, Description, NextLevelDescription);

	// 4개 Broadcast ( bEnableSkillPointsButton, bEnableEquipButton, Description, NextLevelDescription )
	SkillIconSelectedDelegate.Broadcast(bEnableSkillPointsButton, bEnableEquipButton, Description, NextLevelDescription);
}