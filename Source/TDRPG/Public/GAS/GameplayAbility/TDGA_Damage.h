#pragma once
#include "CoreMinimal.h"
#include "GAS/GameplayAbility/TDGA.h"
#include "Library/TDDamageLibrary.h"
#include "GAS/GameplayEffectContext/TDAbilityTypes.h"
#include "Interface/ICombat.h"
#include "TDGA_Damage.generated.h"

/**
 * 
 */
UCLASS()
class TDRPG_API UTDGA_Damage : public UTDGA
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void CauseDamage(AActor* TargetActor);

	FDamageEffectParams SetDamageEffectParams(AActor* TargetActor = nullptr) const;

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
	//*******************************************
};
