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
	// FDamageEffectParams 을 설정한다. TDGA_Damage 자식 클래스에서 '충돌체, 발사체'의 DamageEffectParams 값에 해당 값이 들어가도록 여기의 SetDamageEffectParams()함수를 호출한다.

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
		FRotator Rotation = (TargetActor->GetActorLocation() - GetAvatarActorFromActorInfo()->GetActorLocation()).Rotation(); // 내 기준에서 상대방 방향 회전값
		Rotation.Pitch = 45.f;
		const FVector ToTarget = Rotation.Vector(); // 내 기준에서 상대방 방향
		DamageEffectParams.RagdollImpulse = ToTarget * RagdollImpulseMagnitude;
		DamageEffectParams.KnockbackImpulse = ToTarget * KnockbackImpulseMagnitude;
	}
	if (bRadialDamage) // 반경 데미지
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
		// 몽타주 중 하나를 랜덤으로 골라 반환.
		const int32 Selection = FMath::RandRange(0, TaggedMontages.Num() - 1);
		return TaggedMontages[Selection];
	}

	return FTaggedMontage();
}
