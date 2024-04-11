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

	// ���� Cooldown�� ������(= Cooldwon Tag�� ���������) �˱����� Delegate����ϰ� CooldownTag�� ����� ������ Broadcast �Ѵ�.
	AbilitySystemComponent->RegisterGameplayTagEvent(
		InCooldownTag, EGameplayTagEventType::NewOrRemoved).AddUObject(WaitCooldownChange, &ThisClass::CooldownTagChanged);

	// ���� Cooldown GE�� ����Ǿ����� �˱����� Delegate ����ϰ� ����� ������ Broadcast �Ѵ�.
	AbilitySystemComponent->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(WaitCooldownChange, &ThisClass::OnActiveEffectAdded);

	return WaitCooldownChange;
}

void UTDBPAsync_WaitCooldown::EndTask()
{
	if (false == IsValid(ASC)) return;

	// GameplayTag ��� ����
	ASC->RegisterGameplayTagEvent(
		CooldownTag, EGameplayTagEventType::NewOrRemoved).RemoveAll(this);

	SetReadyToDestroy(); // Game Instance���� ��� ����.
	MarkAsGarbage(); // GC�� �����ذ����� ��ŷ.
}

void UTDBPAsync_WaitCooldown::CooldownTagChanged(const FGameplayTag InCooldownTag, int32 NewCnt)
{
	// Cooldown�� 0(=NewCnt)�� �Ǹ� Broadcast�ϰ� TimeRemaing�� 0�̶� �˸�.
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
					TimeRemaining = TimeRemainingArray[i]; // ���� �������� TimeRemaining�� ����.
				}
			}

			CooldownStart.Broadcast(TimeRemaining);
		}
	}
}
