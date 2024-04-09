#pragma once
#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "TDAbilitySystemComponent.generated.h"

/**
 * 
 */

DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer& /*AssetTags*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FGivenASC, UTDAbilitySystemComponent*);

UCLASS()
class TDRPG_API UTDAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	void AbilityActorInfoSet();
	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities);
	void InputTagPressed(const FGameplayTag& InputTag);
	void InputTagReleased(const FGameplayTag& InputTag);
	void InputTagHeld(const FGameplayTag& InputTag);

	FEffectAssetTags EffectAssetTags;
	FGivenASC GivenASCDelegate;

	bool bStartGivenASC = false;

protected:
	UFUNCTION(Client, Reliable) // Client RPC
	void ClientEffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle);
};
