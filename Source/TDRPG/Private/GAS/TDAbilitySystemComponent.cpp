#include "GAS/TDAbilitySystemComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GAS/GameplayAbility/TDGA.h"
#include "GameplayTags/TDGameplayTags.h"
#include "Interface/IPlayer.h"

void UTDAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UTDAbilitySystemComponent::ClientEffectApplied); // Delegate�� ���ε�. Delegate�� �������� broadcast�Ǹ� �������� �� �Ǿ� Ŭ���̾�Ʈ���� ����ȴ�.
}

// AddCharacterAbilities()�Լ��� ���������� �ҷ�����.
// GivenASCDelegate�� Ŭ���̾�Ʈ�鿡�Ե� Broadcast�ϱ� ���ؼ� �Ʒ��� OnRep_ActivateAbilities()�Լ��� ����Ͽ���.
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

void UTDAbilitySystemComponent::AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupPassiveAbilities)
{
	// �����Ϳ��� �Ҵ��� GameplayAbility�� ��� ���
	for (const TSubclassOf<UGameplayAbility> AbilityClass : StartupPassiveAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		GiveAbilityAndActivateOnce(AbilitySpec); // Activate 
	}
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

void UTDAbilitySystemComponent::EnhanceAttribute(const FGameplayTag& AttributeTag)
{
	IIPlayer* PlayerInterface = Cast<IIPlayer>(GetAvatarActor());
	if (PlayerInterface)
	{
		if (PlayerInterface->GetAttributePoints() > 0)
		{
			ServerEnhanceAttribute(AttributeTag); // Server�� �˷���.
		}
	}
}

void UTDAbilitySystemComponent::ServerEnhanceAttribute_Implementation(const FGameplayTag& AttributeTag)
{
	FGameplayEventData Payload;
	Payload.EventTag = AttributeTag;
	Payload.EventMagnitude = 1.f;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetAvatarActor(), AttributeTag, Payload);

	IIPlayer* PlayerInterface = Cast<IIPlayer>(GetAvatarActor());
	if (PlayerInterface)
	{
		PlayerInterface->AddToAttributePoints(-1);
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

// ���� Ŭ������ AbilitySystemComponent�� ActivateAbilities ���� ����� ������ ȣ��Ǵ� OnRep_ActivateAbilities �Լ��� �������Ͽ� ���.
// GivenASCDelegate�� Ŭ���̾�Ʈ�鿡�Ե� Broadcast�Ѵ�.
void UTDAbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();

	if (false == bStartGivenASC)
	{
		bStartGivenASC = true;
		GivenASCDelegate.Broadcast(this);
	}
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
