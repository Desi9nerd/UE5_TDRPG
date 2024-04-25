#include "UI/WidgetController/TDWidgetControllerOverlay.h"

#include "GameplayTags/TDGameplayTags.h"
#include "GAS/TDAbilitySystemComponent.h"
#include "GAS/TDAttributeSet.h"
#include "GAS/Data/TDDA_Ability.h"
#include "GAS/Data/TDDA_LevelUp.h"
#include "Player/TDPlayerState.h"

void UTDWidgetControllerOverlay::BroadcastInitialValues()
{
	OnHealthChanged.Broadcast(GetTDAttributeSet()->GetHealth());
	OnMaxHealthChanged.Broadcast(GetTDAttributeSet()->GetMaxHealth());
	OnManaChanged.Broadcast(GetTDAttributeSet()->GetMana());
	OnMaxManaChanged.Broadcast(GetTDAttributeSet()->GetMaxMana());
	OnSoulChanged.Broadcast(GetTDAttributeSet()->GetSoul());
}

void UTDWidgetControllerOverlay::BindCallbacksToDependencies() // TDAttributeSet의 데이터와 콜백함수 바인딩
{
	//** Health, MaxHealth가 변경될때 마다 아래함수(HealthChanged, MaxHealthChanged)가 callback됨
	GetASC()->GetGameplayAttributeValueChangeDelegate(
		GetTDAttributeSet()->GetHealthAttribute()).AddUObject(this, &UTDWidgetControllerOverlay::HealthChanged);
	GetASC()->GetGameplayAttributeValueChangeDelegate(
		GetTDAttributeSet()->GetMaxHealthAttribute()).AddUObject(this, &UTDWidgetControllerOverlay::MaxHealthChanged);

	GetASC()->GetGameplayAttributeValueChangeDelegate(
		GetTDAttributeSet()->GetManaAttribute()).AddUObject(this, &UTDWidgetControllerOverlay::ManaChanged);
	GetASC()->GetGameplayAttributeValueChangeDelegate(
		GetTDAttributeSet()->GetMaxManaAttribute()).AddUObject(this, &UTDWidgetControllerOverlay::MaxManaChanged);
	GetASC()->GetGameplayAttributeValueChangeDelegate(
		GetTDAttributeSet()->GetSoulAttribute()).AddUObject(this, &UTDWidgetControllerOverlay::SoulChanged);


	/*Cast<UTDAbilitySystemComponent>(AbilitySystemComponent)->EffectAssetTags.AddLambda(
		[this](const FGameplayTagContainer& AssetTags)
		{
			for (const FGameplayTag& Tag : AssetTags)
			{
				const FString Msg = FString::Printf(TEXT("GE Tag: %s"), *Tag.ToString());
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, Msg);

				FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
			}
		}
	);*/ // 위: 람다함수 버젼, 아래: 함수를 밖으로 뺀 버젼 
	if (GetTDASC())
	{
		GetTDASC()->EquippedAbilityDelegate.AddUObject(this, &ThisClass::OnEquippedAbility);

		if (GetTDASC()->bStartGivenASC) // AbilitySystemComponent 데이터가 적용되어 있다면
		{
			BroadcastDA_AbilityInfo();
		}
		else // AbilitySystemComponent 데이터가 적용이 안 되어 있다면
		{
			GetTDASC()->GivenASCDelegate.AddUObject(this, &UTDWidgetControllerOverlay::BroadcastDA_AbilityInfo);
		}

		GetTDASC()->EffectAssetTagsDelegate.AddUObject(this, &UTDWidgetControllerOverlay::ReadDataTableRowByTag);
	}

	
	GetTDPlayerState()->OnExpChangedDelegate.AddUObject(this, &UTDWidgetControllerOverlay::OnExpChanged);
	GetTDPlayerState()->OnPlayerLevelChangedDelegate.AddUObject(this, &UTDWidgetControllerOverlay::OnPlayerLevelChanged);
}

void UTDWidgetControllerOverlay::ReadDataTableRowByTag(const FGameplayTagContainer& AssetTags)
{
	for (const FGameplayTag& Tag : AssetTags)
	{
		FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
		if (Tag.MatchesTag(MessageTag)) // MatchesTag로 "Message"글자를 포함하고 있는지 확인
		{
			const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
			MessageWidgetRowDelegate.Broadcast(*Row); // Delegate Broadcast
		}
	}
}

void UTDWidgetControllerOverlay::HealthChanged(const FOnAttributeChangeData& Data) const
{
	OnHealthChanged.Broadcast(Data.NewValue);
}

void UTDWidgetControllerOverlay::MaxHealthChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxHealthChanged.Broadcast(Data.NewValue);
}

void UTDWidgetControllerOverlay::ManaChanged(const FOnAttributeChangeData& Data) const
{
	OnManaChanged.Broadcast(Data.NewValue);
}

void UTDWidgetControllerOverlay::MaxManaChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxManaChanged.Broadcast(Data.NewValue);
}

void UTDWidgetControllerOverlay::SoulChanged(const FOnAttributeChangeData& Data) const
{
	OnSoulChanged.Broadcast(Data.NewValue);
}

void UTDWidgetControllerOverlay::OnExpChanged(int32 InNewExp)
{
	const UTDDA_LevelUp* TDDA_LevelUpInfo = GetTDPlayerState()->TDDA_LevelUpInfo;
	checkf(TDDA_LevelUpInfo, TEXT("No TDDA_LevelUpInfo. Check: UTDWidgetControllerOverlay::OnExpChanged & TDPlayerState BP"));

	const int32 PlayerLevel = TDDA_LevelUpInfo->FindDA_LevelUpForExp(InNewExp);
	const int32 MaxPlayerLevel = TDDA_LevelUpInfo->DA_LevelUpInfo.Num();

	if (PlayerLevel <= MaxPlayerLevel && PlayerLevel > 0)
	{
		const int32 LevelUpRequirement = TDDA_LevelUpInfo->DA_LevelUpInfo[PlayerLevel].LevelUpRequirement;
		const int32 PreviousLevelUpRequirement = TDDA_LevelUpInfo->DA_LevelUpInfo[PlayerLevel - 1].LevelUpRequirement;

		const int32 DeltaLevelRequirement = LevelUpRequirement - PreviousLevelUpRequirement;
		const int32 ExpForThisLevel = InNewExp - PreviousLevelUpRequirement;

		const float ExpBarPercent = static_cast<float>(ExpForThisLevel) / static_cast<float>(DeltaLevelRequirement);

		OnExpPercentChangedDelegate.Broadcast(ExpBarPercent);
	}
}

void UTDWidgetControllerOverlay::OnPlayerLevelChanged(int32 InNewPlayerLevel) const
{
	OnPlayerLevelChangedDelegate.Broadcast(InNewPlayerLevel);
}

void UTDWidgetControllerOverlay::OnEquippedAbility(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, const FGameplayTag& SlotTag, const FGameplayTag& PreviousSlotTag) const
{
	//****************************************************************************
	//** Old Slot
	FDA_Ability DA_AbilityInfo_LastSlot;
	DA_AbilityInfo_LastSlot.StatusTag = FTDGameplayTags::GetTDGameplayTags().Abilities_Status_Unlocked;
	DA_AbilityInfo_LastSlot.InputTag = PreviousSlotTag;
	DA_AbilityInfo_LastSlot.AbilityTag = FTDGameplayTags::GetTDGameplayTags().Abilities_None;

	DA_AbilityInfoDelegate.Broadcast(DA_AbilityInfo_LastSlot);
	//****************************************************************************

	//****************************************************************************
	//** New Slot
	FDA_Ability DA_AbilityInfo = TDDA_Ability->FindDA_AbilityForTag(AbilityTag);
	DA_AbilityInfo.StatusTag = StatusTag;
	DA_AbilityInfo.InputTag = SlotTag;

	DA_AbilityInfoDelegate.Broadcast(DA_AbilityInfo);
	//****************************************************************************
}
