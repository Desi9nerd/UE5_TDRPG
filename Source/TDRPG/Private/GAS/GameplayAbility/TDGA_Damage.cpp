#include "GAS/GameplayAbility/TDGA_Damage.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

void UTDGA_Damage::CauseDamage(AActor* TargetActor)
{
	FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(GameplayEffectDamageClass, 1.f);
	for(TTuple<FGameplayTag, FScalableFloat> Pair : DamageTypes)
	{
		const float ScaledDamage = Pair.Value.GetValueAtLevel(GetAbilityLevel());
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, Pair.Key, ScaledDamage);
	}

	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data.Get(), UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));
}

// DamageType�� �˻�. TMap<FGameplayTag, FScalableFloat> DamageTypes ���� �ش� DamageType�� GameplayTag�� �ִ��� �˻��ϰ� Value���� ����.
float UTDGA_Damage::GetDamageByDamageType(float InAbilityLevel, const FGameplayTag& DamageType)
{
	checkf(DamageTypes.Contains(DamageType), TEXT("GameplayAbilit [%s] does not contain DamageType [%s]. Check: UTDGA_Damage::GetDamageByDamageType"), *GetNameSafe(this), *DamageType.ToString());

	return DamageTypes[DamageType].GetValueAtLevel(InAbilityLevel);
}

FTaggedMontage UTDGA_Damage::GetRandomTaggedMontageFromArray(const TArray<FTaggedMontage>& TaggedMontages) const
{
	if (TaggedMontages.Num() > 0)
	{
		// ��Ÿ�� �� �ϳ��� �������� ��� ��ȯ.
		const int32 Selection = FMath::RandRange(0, TaggedMontages.Num() - 1);
		return TaggedMontages[Selection];
	}

	return FTaggedMontage();
}
