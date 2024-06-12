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

	FDamageEffectParams DEffectParams;
	DEffectParams.WorldContextObject = GetAvatarActorFromActorInfo();
	DEffectParams.GEDamageClass = GEDamageClass;
	DEffectParams.SourceAbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	DEffectParams.TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	DEffectParams.BaseDamage = DamageScalableFloat.GetValueAtLevel(GetAbilityLevel());
	DEffectParams.AbilityLevel = GetAbilityLevel();
	DEffectParams.DamageType = DamageType;
	DEffectParams.DebuffChance = DebuffChance;
	DEffectParams.DebuffDamage = DebuffDamage;
	DEffectParams.DebuffDuration = DebuffDuration;
	DEffectParams.DebuffFrequency = DebuffFrequency;
	DEffectParams.RagdollImpulseMagnitude = RagdollImpulseMagnitude;
	DEffectParams.KnockbackImpulseMagnitude = KnockbackImpulseMagnitude;
	DEffectParams.KnockbackChance = KnockbackChance;
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
			DEffectParams.KnockbackImpulse = ToTarget * KnockbackImpulseMagnitude;
		}
		if (false == bOverrideRagdollImpulse) // Ragdoll ���� ���밪 X
		{
			DEffectParams.RagdollImpulse = ToTarget * RagdollImpulseMagnitude;
		}
	}


	if (bOverrideKnockbackDirection) // Knockback ���� ���� O
	{
		KnockbackDirectionOverride.Normalize();
		DEffectParams.KnockbackImpulse = KnockbackDirectionOverride * KnockbackImpulseMagnitude;
		if (bOverridePitch)
		{
			FRotator KnockbackRotation = KnockbackDirectionOverride.Rotation();
			KnockbackRotation.Pitch = PitchOverride;
			DEffectParams.KnockbackImpulse = KnockbackRotation.Vector() * KnockbackImpulseMagnitude;
		}
	}
	if (bOverrideRagdollImpulse) // Ragdoll ���� ���� O
	{
		RagdollImpulseDirectionOverride.Normalize();
		DEffectParams.RagdollImpulse = RagdollImpulseDirectionOverride * RagdollImpulseMagnitude;
		if (bOverridePitch)
		{
			FRotator RagdollImpulseRotation = RagdollImpulseDirectionOverride.Rotation();
			RagdollImpulseRotation.Pitch = PitchOverride;
			DEffectParams.RagdollImpulse = RagdollImpulseRotation.Vector() * RagdollImpulseMagnitude;
		}
	}
	if (bRadialDamage) // �ݰ� ������
	{
		DEffectParams.bRadialDamage = bRadialDamage;
		DEffectParams.RadialDamageOrigin = InRadialDamageOrigin;
		DEffectParams.RadialDamageInnerRadius = RadialDamageInnerRadius;
		DEffectParams.RadialDamageOuterRadius = RadialDamageOuterRadius;
	}

	return DEffectParams;
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
