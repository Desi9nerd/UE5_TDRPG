#include "GAS/TDAbilitySystemComponent.h"
#include "GAS/GameplayAbility/TDGA.h"
#include "GameplayTags/TDGameplayTags.h"

void UTDAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UTDAbilitySystemComponent::ClientEffectApplied); // Delegate�� ���ε�. Delegate�� �������� broadcast�Ǹ� �������� �� �Ǿ� Ŭ���̾�Ʈ���� ����ȴ�.
}

void UTDAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	// �����Ϳ��� �Ҵ��� GameplayAbility�� ��� ���
	for (const TSubclassOf<UGameplayAbility> AbilityClass : StartupAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		const TObjectPtr<UTDGA> TDAbility = Cast<UTDGA>(AbilitySpec.Ability);
		if (IsValid(TDAbility))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(TDAbility->StartupInputTag);
			GiveAbility(AbilitySpec);
		}
	}

	bStartGivenASC = true;
	GivenASCDelegate.Broadcast(this);
}

void UTDAbilitySystemComponent::InputTagPressed(const FGameplayTag& InputTag)
{
	if (false == InputTag.IsValid()) return;

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputReleased(AbilitySpec);
		}
	}
}

void UTDAbilitySystemComponent::InputTagReleased(const FGameplayTag& InputTag)
{
	if (false == InputTag.IsValid()) return;

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputReleased(AbilitySpec);
		}
	}
}

void UTDAbilitySystemComponent::InputTagHeld(const FGameplayTag& InputTag)
{
	if (false == InputTag.IsValid()) return;

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec); // �����Լ�.
			if (false == AbilitySpec.IsActive())
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
}

void UTDAbilitySystemComponent::ForEachAbility(const FForEachAbility& Delegate)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (false == Delegate.ExecuteIfBound(AbilitySpec))
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to execute delegate in %hs"), __FUNCTION__);
		}
	}
}

FGameplayTag UTDAbilitySystemComponent::GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	if (AbilitySpec.Ability)
	{
		for (FGameplayTag Tag : AbilitySpec.Ability.Get()->AbilityTags)
		{
			if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities")))) // "Abilities"��� �̸��� �ִٸ�
			{
				return Tag;
			}
		}
	}
	return FGameplayTag();
}

FGameplayTag UTDAbilitySystemComponent::GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for (FGameplayTag Tag : AbilitySpec.DynamicAbilityTags)
	{
		if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("InputTag"))))
		{
			return Tag;
		}
	}
	return FGameplayTag();
}

// AbilitySystemComponent.h 107��
// DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnGameplayEffectAppliedDelegate, UAbilitySystemComponent*, const FGameplayEffectSpec&, FActiveGameplayEffectHandle);�� ����ϱ� ���� 3���� �Ű����� ���
// FOnGameplayEffectAppliedDelegate OnGameplayEffectAppliedDelegateToSelf;
// �����ϸ�, �� ASC�� GameplayEffect�� ����Ǹ� �Ʒ��� �Լ��� �ݹ�Ǿ� ����ȴ�.
void UTDAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent,
	const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);

	EffectAssetTags.Broadcast(TagContainer);
}
