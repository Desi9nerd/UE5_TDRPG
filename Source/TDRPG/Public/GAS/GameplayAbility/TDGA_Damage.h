#pragma once
#include "CoreMinimal.h"
#include "GAS/GameplayAbility/TDGA.h"
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

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> GameplayEffectDamageClass;

	//*******************************************
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag DamageType;
	UPROPERTY(EditDefaultsOnly)
	FScalableFloat DamageScalableFloat;
	//*******************************************

	UFUNCTION(BlueprintCallable)
	FTaggedMontage GetRandomTaggedMontageFromArray(const TArray<FTaggedMontage>& TaggedMontages) const;
};
