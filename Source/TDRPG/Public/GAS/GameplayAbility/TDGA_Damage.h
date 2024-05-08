#pragma once
#include "CoreMinimal.h"
#include "GAS/GameplayAbility/TDGA.h"
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
	float DebuffChance = 0.f;
	UPROPERTY(EditDefaultsOnly)
	float DebuffDamage = 0.f;
	UPROPERTY(EditDefaultsOnly)
	float DebuffFrequency = 1.f;
	UPROPERTY(EditDefaultsOnly)
	float DebuffDuration = 0.f;
	//*******************************************
};
