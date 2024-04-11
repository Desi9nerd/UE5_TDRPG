#include "GAS/BPAsyncTask/TDBPAsync_WaitCooldown.h"
#include "AbilitySystemComponent.h"

UTDBPAsync_WaitCooldown* UTDBPAsync_WaitCooldown::WaitForCooldownChange(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& InCooldownTag)
{
	UTDBPAsync_WaitCooldown* WaitCooldownChange = NewObject<UTDBPAsync_WaitCooldown>();
	WaitCooldownChange->ASC = AbilitySystemComponent;
	WaitCooldownChange->CooldownTag = InCooldownTag;

	if (false == IsValid(AbilitySystemComponent) || false == InCooldownTag.IsValid())
	{
		WaitCooldownChange->EndTask();
		return nullptr;
	}

	// 언제 Cooldown이 끝난지(= Cooldwon Tag가 사라졌는지) 알기위해 Delegate등록하고 CooldownTag가 변경된 시점에 Broadcast 한다.
	AbilitySystemComponent->RegisterGameplayTagEvent(
		InCooldownTag, EGameplayTagEventType::NewOrRemoved).AddUObject(WaitCooldownChange, &ThisClass::CooldownTagChanged);

	// 언제 Cooldown GE가 적용되었는지 알기위해 Delegate 등록하고 적용된 시점에 Broadcast 한다.
	AbilitySystemComponent->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(WaitCooldownChange, &ThisClass::OnActiveEffectAdded);

	return WaitCooldownChange;
}

void UTDBPAsync_WaitCooldown::EndTask()
{
	if (false == IsValid(ASC)) return;

	// GameplayTag 등록 해제
	ASC->RegisterGameplayTagEvent(
		CooldownTag, EGameplayTagEventType::NewOrRemoved).RemoveAll(this);

	SetReadyToDestroy(); // Game Instance에서 등록 해제.
	MarkAsGarbage(); // GC가 수거해가도록 마킹.
}

void UTDBPAsync_WaitCooldown::CooldownTagChanged(const FGameplayTag InCooldownTag, int32 NewCnt)
{
	// Cooldown이 0(=NewCnt)이 되면 Broadcast하고 TimeRemaing이 0이라 알림.
	if (NewCnt == 0)
	{
		CooldownEnd.Broadcast(0.f);
	}
}

void UTDBPAsync_WaitCooldown::OnActiveEffectAdded(UAbilitySystemComponent* TargetASC, const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer AssetTags;
	SpecApplied.GetAllAssetTags(AssetTags);

	FGameplayTagContainer GrantedTags;
	SpecApplied.GetAllGrantedTags(GrantedTags);

	if (AssetTags.HasTagExact(CooldownTag) || GrantedTags.HasTagExact(CooldownTag))
	{
		FGameplayEffectQuery GameplayEffectQuery = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(CooldownTag.GetSingleTagContainer());
		TArray<float> TimeRemainingArray = ASC->GetActiveEffectsTimeRemaining(GameplayEffectQuery);
		if (TimeRemainingArray.Num() > 0)
		{
			float TimeRemaining = TimeRemainingArray[0];
			for (int32 i = 0; i < TimeRemainingArray.Num(); i++)
			{
				if (TimeRemainingArray[i] > TimeRemaining)
				{
					TimeRemaining = TimeRemainingArray[i]; // 가장 높은값을 TimeRemaining로 설정.
				}
			}

			CooldownStart.Broadcast(TimeRemaining);
		}
	}
}
