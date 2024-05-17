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

FDamageEffectParams UTDGA_Damage::SetDamageEffectParams(AActor* TargetActor, FVector InRadialDamageOrigin, bool bOverrideKnockbackDirection, FVector KnockbackDirectionOverride, bool bOverrideRagdollImpulse, FVector RagdollImpulseDirectionOverride, bool bOverridePitch, float PitchOverride) const
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
		if (bOverridePitch)
		{
			Rotation.Pitch = PitchOverride;
		}

		const FVector ToTarget = Rotation.Vector(); // �� ���ؿ��� ���� ����

		if (false == bOverrideKnockbackDirection) 
		{
			DamageEffectParams.KnockbackImpulse = ToTarget * KnockbackImpulseMagnitude;
		}
		if (false == bOverrideRagdollImpulse) // Ragdoll ���� ���밪 X
		{
			DamageEffectParams.RagdollImpulse = ToTarget * RagdollImpulseMagnitude;
		}
	}


	if (bOverrideRagdollImpulse) // Ragdoll ���� ���� O
	{
		RagdollImpulseDirectionOverride.Normalize();
		DamageEffectParams.RagdollImpulse = RagdollImpulseDirectionOverride * RagdollImpulseMagnitude;
		if (bOverridePitch)
		{
			FRotator RagdollImpulseRotation = RagdollImpulseDirectionOverride.Rotation();
			RagdollImpulseRotation.Pitch = PitchOverride;
			DamageEffectParams.RagdollImpulse = RagdollImpulseRotation.Vector() * RagdollImpulseMagnitude;
		}
	}
	if (bOverrideKnockbackDirection) // Knockback ���� ���� O
	{
		KnockbackDirectionOverride.Normalize();
		DamageEffectParams.KnockbackImpulse = KnockbackDirectionOverride * KnockbackImpulseMagnitude;
		if (bOverridePitch)
		{
			FRotator KnockbackRotation = KnockbackDirectionOverride.Rotation();
			KnockbackRotation.Pitch = PitchOverride;
			DamageEffectParams.KnockbackImpulse = KnockbackRotation.Vector() * KnockbackImpulseMagnitude;
		}
	}
	if (bRadialDamage) // �ݰ� ������
	{
		DamageEffectParams.bRadialDamage = bRadialDamage;
		DamageEffectParams.RadialDamageOrigin = InRadialDamageOrigin;
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
