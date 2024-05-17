#include "GAS/GameplayAbility/TDGA_Damage.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

void UTDGA_Damage::CauseDamage(AActor* TargetActor)
{
	FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(GEDamageClass, 1.f);

	const float ScaledDamage = DamageScalableFloat.GetValueAtLevel(GetAbilityLevel());
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, DamageType, ScaledDamage);

	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data.Get(), UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));
}

FDamageEffectParams UTDGA_Damage::SetDamageEffectParams(AActor* TargetActor) const
{
	// FDamageEffectParams �� �����Ѵ�. TDGA_Damage �ڽ� Ŭ�������� '�浹ü, �߻�ü'�� DamageEffectParams ���� �ش� ���� ������ ������ SetDamageEffectParams()�Լ��� ȣ���Ѵ�.

	FDamageEffectParams DamageEffectParams;
	DamageEffectParams.WorldContextObject = GetAvatarActorFromActorInfo();
	DamageEffectParams.GEDamageClass = GEDamageClass;
	DamageEffectParams.SourceAbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	DamageEffectParams.TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	DamageEffectParams.BaseDamage = DamageScalableFloat.GetValueAtLevel(GetAbilityLevel());
	DamageEffectParams.AbilityLevel = GetAbilityLevel();
	DamageEffectParams.DamageType = DamageType;
	DamageEffectParams.DebuffChance = DebuffChance;
	DamageEffectParams.DebuffDamage = DebuffDamage;
	DamageEffectParams.DebuffDuration = DebuffDuration;
	DamageEffectParams.DebuffFrequency = DebuffFrequency;
	DamageEffectParams.RagdollImpulseMagnitude = RagdollImpulseMagnitude;
	DamageEffectParams.KnockbackImpulseMagnitude = KnockbackImpulseMagnitude;
	DamageEffectParams.KnockbackChance = KnockbackChance;
	if (IsValid(TargetActor))
	{
		FRotator Rotation = (TargetActor->GetActorLocation() - GetAvatarActorFromActorInfo()->GetActorLocation()).Rotation(); // �� ���ؿ��� ���� ���� ȸ����
		Rotation.Pitch = 45.f;
		const FVector ToTarget = Rotation.Vector(); // �� ���ؿ��� ���� ����
		DamageEffectParams.RagdollImpulse = ToTarget * RagdollImpulseMagnitude;
		DamageEffectParams.KnockbackImpulse = ToTarget * KnockbackImpulseMagnitude;
	}
	if (bRadialDamage) // �ݰ� ������
	{
		DamageEffectParams.bRadialDamage = bRadialDamage;
		DamageEffectParams.RadialDamageOrigin = RadialDamageOrigin;
		DamageEffectParams.RadialDamageInnerRadius = RadialDamageInnerRadius;
		DamageEffectParams.RadialDamageOuterRadius = RadialDamageOuterRadius;
	}

	return DamageEffectParams;
}

float UTDGA_Damage::GetDamageAtCurrentAbilityLevel() const
{
	return DamageScalableFloat.GetValueAtLevel(GetAbilityLevel());
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
