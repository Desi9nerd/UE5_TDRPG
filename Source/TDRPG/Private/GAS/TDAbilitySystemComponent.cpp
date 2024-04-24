#include "GAS/TDAbilitySystemComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GAS/GameplayAbility/TDGA.h"
#include "GameplayTags/TDGameplayTags.h"
#include "GAS/TDAbilitySystemBPLibrary.h"
#include "GAS/Data/TDDA_Ability.h"
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
			AbilitySpec.DynamicAbilityTags.AddTag(TDAbility->StartupInputTag); // InputTag ���
			AbilitySpec.DynamicAbilityTags.AddTag(FTDGameplayTags::GetTDGameplayTags().Abilities_Status_Equipped); // Abilities_Status_Eligible �±� ���
			GiveAbility(AbilitySpec);
			//MarkAbilitySpecDirty(AbilitySpec);
		}
	}

	bStartGivenASC = true;
	GivenASCDelegate.Broadcast();
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

// �����Ƽ ������Ʈ. ���ο� �����Ƽ �߰��Ұ� �ִ��� Ȯ���ϰ� �߰��ϱ�.
void UTDAbilitySystemComponent::UpdateAbilityStatuses(int32 PlayerLevel)
{
	UTDDA_Ability* TDDA_Ability = UTDAbilitySystemBPLibrary::GetTDDA_Ability(GetAvatarActor());
	for (const FDA_Ability& DA_Ability_Iter : TDDA_Ability->DA_AbilityInfo)
	{
		if (false == DA_Ability_Iter.AbilityTag.IsValid()) continue; // AbilityTag�� ���ٸ� continue.
		if (PlayerLevel < DA_Ability_Iter.LevelRequirement) continue; // PlayerLevel�� �������� ���� ���ٸ� continue.

		if (GetSpecFromAbilityTag(DA_Ability_Iter.AbilityTag) == nullptr)
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(DA_Ability_Iter.Ability, 1);
			AbilitySpec.DynamicAbilityTags.AddTag((FTDGameplayTags::GetTDGameplayTags().Abilities_Status_Eligible)); // ��Ÿ�ӿ� �±� �߰�.
			GiveAbility(AbilitySpec); // �����Ƽ �ο�.
			MarkAbilitySpecDirty(AbilitySpec);
			ClientUpdateAbilityStatus(DA_Ability_Iter.AbilityTag, FTDGameplayTags::GetTDGameplayTags().Abilities_Status_Eligible);
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

FGameplayTag UTDAbilitySystemComponent::GetStatusFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for (FGameplayTag StatusTag : AbilitySpec.DynamicAbilityTags)
	{
		if (StatusTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities.Status"))))
		{
			return StatusTag;
		}
	}
	return FGameplayTag();
}

FGameplayAbilitySpec* UTDAbilitySystemComponent::GetSpecFromAbilityTag(const FGameplayTag& AbilityTag)
{
	/** Used to stop us from removing abilities from an ability system component while we're iterating through the abilities */
	FScopedAbilityListLock ActiveScopeLoc(*this);

	for (FGameplayAbilitySpec& AbilitySpecIter : GetActivatableAbilities())
	{
		for (FGameplayTag TagIter : AbilitySpecIter.Ability.Get()->AbilityTags)
		{
			if (TagIter.MatchesTag(AbilityTag))
			{
				// ActivatableAbilities �� AbilityTag�� ��ġ�ϴ� �±׸� ������ �ִ°� �ִٸ� AbilitySpec�� ����.
				return &AbilitySpecIter;
			}
		}
	}

	return nullptr;
}


void UTDAbilitySystemComponent::ClientUpdateAbilityStatus_Implementation(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag)
{
	AbilityStatusChanged.Broadcast(AbilityTag, StatusTag);
}

// ���� Ŭ������ AbilitySystemComponent�� ActivateAbilities ���� ����� ������ ȣ��Ǵ� OnRep_ActivateAbilities �Լ��� �������Ͽ� ���.
// GivenASCDelegate�� Ŭ���̾�Ʈ�鿡�Ե� Broadcast�Ѵ�.
void UTDAbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();

	if (false == bStartGivenASC)
	{
		bStartGivenASC = true;
		GivenASCDelegate.Broadcast();
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
