#include "GAS/AbilitySystemGlobals/TDAbilitySystemGlobals.h"
#include "GAS/GameplayEffectContext/TDAbilityTypes.h"

FGameplayEffectContext* UTDAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FTDGameplayEffectContext(); // ���� ���� FTDGameplayEffectContext Ÿ������ ����
}
