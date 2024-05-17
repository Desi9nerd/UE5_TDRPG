#pragma once
#include "CoreMinimal.h"
#include "GAS/GameplayAbility/TDGA.h"
#include "Library/TDDamageLibrary.h"
#include "GAS/GameplayEffectContext/TDAbilityTypes.h"
#include "Interface/ICombat.h"
#include "TDGA_Damage.generated.h"

/** 공격 스킬에 공통으로 들어가는 데미지 관련 처리를 하는 GameplayAbility 클래스
 * 
 */
UCLASS()
class TDRPG_API UTDGA_Damage : public UTDGA
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void CauseDamage(AActor* TargetActor);

	UFUNCTION(BlueprintPure)
	FDamageEffectParams SetDamageEffectParams(AActor* TargetActor = nullptr, FVector InRadialDamageOrigin = FVector::ZeroVector, bool bOverrideKnockbackDirection = false, FVector KnockbackDirectionOverride = FVector::ZeroVector, bool bOverrideRagdollImpulse = false, FVector RagdollImpulseDirectionOverride = FVector::ZeroVector, bool bOverridePitch = false, float PitchOverride = 0.f) const;

	UFUNCTION(BlueprintPure)
	float GetDamageAtCurrentAbilityLevel() const;

protected:
	UFUNCTION(BlueprintCallable)
	FTaggedMontage GetRandomTaggedMontageFromArray(const TArray<FTaggedMontage>& TaggedMontages) const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> GEDamageClass;

	//*******************************************
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag DamageType;
	UPROPERTY(EditDefaultsOnly)
	FScalableFloat DamageScalableFloat;

	UPROPERTY(EditDefaultsOnly)
	float DebuffChance = 50.f;
	UPROPERTY(EditDefaultsOnly)
	float DebuffDamage = 3.f;
	UPROPERTY(EditDefaultsOnly)
	float DebuffFrequency = 1.f;
	UPROPERTY(EditDefaultsOnly)
	float DebuffDuration = 3.f;
	UPROPERTY(EditDefaultsOnly)
	float RagdollImpulseMagnitude = 10000.f;
	UPROPERTY(EditDefaultsOnly)
	float KnockbackImpulseMagnitude = 1000.f;
	UPROPERTY(EditDefaultsOnly)
	float KnockbackChance = 0.f;

	UPROPERTY(EditDefaultsOnly)
	bool bRadialDamage = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float RadialDamageInnerRadius = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float RadialDamageOuterRadius = 0.f;
	//*******************************************
};
