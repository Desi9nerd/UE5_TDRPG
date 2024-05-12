#pragma once
#include "CoreMinimal.h"
#include "GAS/GameplayAbility/TDGA_DamageProjectile.h"
#include "TDGA_DamageProjectile_Arrow.generated.h"

/**
 * 
 */
UCLASS()
class TDRPG_API UTDGA_DamageProjectile_Arrow : public UTDGA_DamageProjectile
{
	GENERATED_BODY()

public:
	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextAbilityLevelDescription(int32 AbilityLevel) override;

	UFUNCTION(BlueprintCallable)
	void SpawnProjectiles(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, bool bOverridePitch, float PitchOverride, AActor* HomingTarget);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "FireBolt")
	float ArrowSpread = 90.f;

	UPROPERTY(EditDefaultsOnly, Category = "FireBolt")
	int32 MaxNumArrows = 5;
};
