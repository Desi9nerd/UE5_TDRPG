#include "GAS/TDAbilitySystemComponent.h"

void UTDAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UTDAbilitySystemComponent::EffectApplied); // Delegate�� ���ε�
}

// AbilitySystemComponent.h 107��
// DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnGameplayEffectAppliedDelegate, UAbilitySystemComponent*, const FGameplayEffectSpec&, FActiveGameplayEffectHandle);�� ����ϱ� ���� 3���� �Ű����� ���
// FOnGameplayEffectAppliedDelegate OnGameplayEffectAppliedDelegateToSelf;
// �����ϸ�, �� ASC�� GameplayEffect�� ����Ǹ� �Ʒ��� �Լ��� �ݹ�Ǿ� ����ȴ�.
void UTDAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent,
	const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);

	EffectAssetTags.Broadcast(TagContainer);
}
