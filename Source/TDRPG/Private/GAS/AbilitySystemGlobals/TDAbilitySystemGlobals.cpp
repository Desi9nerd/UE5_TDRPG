#include "GAS/AbilitySystemGlobals/TDAbilitySystemGlobals.h"
#include "GAS/GameplayEffectContext/TDAbilityTypes.h"

FGameplayEffectContext* UTDAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FTDGameplayEffectContext(); // 내가 만든 FTDGameplayEffectContext 타입으로 리턴
}
