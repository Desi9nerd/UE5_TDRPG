#include "GAS/TDAbilitySystemComponent.h"

void UTDAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UTDAbilitySystemComponent::EffectApplied); // Delegate에 바인딩
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
