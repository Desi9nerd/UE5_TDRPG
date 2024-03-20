#include "GAS/TDAbilitySystemComponent.h"

void UTDAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UTDAbilitySystemComponent::EffectApplied); // Delegate에 바인딩
}

void UTDAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	// 에디터에서 할당한 GameplayAbility를 모두 등록
	for (TSubclassOf<UGameplayAbility> AbilityClass : StartupAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		GiveAbilityAndActivateOnce(AbilitySpec);
	}
}

// AbilitySystemComponent.h 107줄
// DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnGameplayEffectAppliedDelegate, UAbilitySystemComponent*, const FGameplayEffectSpec&, FActiveGameplayEffectHandle);에 사용하기 위해 3개의 매개변수 사용
// FOnGameplayEffectAppliedDelegate OnGameplayEffectAppliedDelegateToSelf;
// 정리하면, 이 ASC에 GameplayEffect가 적용되면 아래의 함수가 콜백되어 실행된다.
void UTDAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent,
	const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);

	EffectAssetTags.Broadcast(TagContainer);
}
