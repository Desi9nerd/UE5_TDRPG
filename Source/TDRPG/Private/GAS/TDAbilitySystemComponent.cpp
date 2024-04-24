#include "GAS/TDAbilitySystemComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GAS/GameplayAbility/TDGA.h"
#include "GameplayTags/TDGameplayTags.h"
#include "GAS/TDAbilitySystemBPLibrary.h"
#include "GAS/Data/TDDA_Ability.h"
#include "Interface/IPlayer.h"

void UTDAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UTDAbilitySystemComponent::ClientEffectApplied); // Delegate에 바인딩. Delegate가 서버에서 broadcast되면 서버에서 콜 되어 클라이언트에서 실행된다.
}

// AddCharacterAbilities()함수는 서버에서만 불려진다.
// GivenASCDelegate을 클라이언트들에게도 Broadcast하기 위해서 아래의 OnRep_ActivateAbilities()함수를 사용하였다.
void UTDAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	// 에디터에서 할당한 GameplayAbility를 모두 등록
	for (const TSubclassOf<UGameplayAbility> AbilityClass : StartupAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		const TObjectPtr<UTDGA> TDAbility = Cast<UTDGA>(AbilitySpec.Ability);
		if (IsValid(TDAbility))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(TDAbility->StartupInputTag); // InputTag 등록
			AbilitySpec.DynamicAbilityTags.AddTag(FTDGameplayTags::GetTDGameplayTags().Abilities_Status_Equipped); // Abilities_Status_Eligible 태그 등록
			GiveAbility(AbilitySpec);
			//MarkAbilitySpecDirty(AbilitySpec);
		}
	}

	bStartGivenASC = true;
	GivenASCDelegate.Broadcast();
}

void UTDAbilitySystemComponent::AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupPassiveAbilities)
{
	// 에디터에서 할당한 GameplayAbility를 모두 등록
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
			AbilitySpecInputPressed(AbilitySpec); // 내장함수.
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
			ServerEnhanceAttribute(AttributeTag); // Server에 알려줌.
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

// 어빌리티 업데이트. 새로운 어빌리티 추가할게 있는지 확인하고 추가하기.
void UTDAbilitySystemComponent::UpdateAbilityStatuses(int32 PlayerLevel)
{
	UTDDA_Ability* TDDA_Ability = UTDAbilitySystemBPLibrary::GetTDDA_Ability(GetAvatarActor());
	for (const FDA_Ability& DA_Ability_Iter : TDDA_Ability->DA_AbilityInfo)
	{
		if (false == DA_Ability_Iter.AbilityTag.IsValid()) continue; // AbilityTag가 없다면 continue.
		if (PlayerLevel < DA_Ability_Iter.LevelRequirement) continue; // PlayerLevel이 레벨조건 보다 낮다면 continue.

		if (GetSpecFromAbilityTag(DA_Ability_Iter.AbilityTag) == nullptr)
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(DA_Ability_Iter.Ability, 1);
			AbilitySpec.DynamicAbilityTags.AddTag((FTDGameplayTags::GetTDGameplayTags().Abilities_Status_Eligible)); // 런타임에 태그 추가.
			GiveAbility(AbilitySpec); // 어빌리티 부여.
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
			if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities")))) // "Abilities"라는 이름이 있다면
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
				// ActivatableAbilities 중 AbilityTag와 일치하는 태그를 가지고 있는게 있다면 AbilitySpec를 리턴.
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

// 상위 클래스인 AbilitySystemComponent에 ActivateAbilities 값이 변경될 때마다 호출되는 OnRep_ActivateAbilities 함수를 재정의하여 사용.
// GivenASCDelegate을 클라이언트들에게도 Broadcast한다.
void UTDAbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();

	if (false == bStartGivenASC)
	{
		bStartGivenASC = true;
		GivenASCDelegate.Broadcast();
	}
}

// AbilitySystemComponent.h 107줄
// DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnGameplayEffectAppliedDelegate, UAbilitySystemComponent*, const FGameplayEffectSpec&, FActiveGameplayEffectHandle);에 사용하기 위해 3개의 매개변수 사용
// FOnGameplayEffectAppliedDelegate OnGameplayEffectAppliedDelegateToSelf;
// 정리하면, 이 ASC에 GameplayEffect가 적용되면 아래의 함수가 콜백되어 실행된다.
void UTDAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent,
	const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);

	EffectAssetTags.Broadcast(TagContainer);
}
