#pragma once
#include "CoreMinimal.h"
#include "GAS/GameplayAbility/TDGA.h"
#include "TDGA_Projectile.generated.h"

class ATDProjectile;

/**
 * 
 */

UCLASS()
class TDRPG_API UTDGA_Projectile : public UTDGA
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	//virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void SpawnProjectile();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ATDProjectile> ProjectileClass; // Projectile Actor

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag EventTag;
};
