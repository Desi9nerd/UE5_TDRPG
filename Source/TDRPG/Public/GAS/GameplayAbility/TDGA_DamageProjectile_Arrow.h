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
	UPROPERTY(EditDefaultsOnly)
	float ArrowSpread = 45.f;

	UPROPERTY(EditDefaultsOnly)
	int32 MaxNumArrows = 5;

	UPROPERTY(EditDefaultsOnly)
	float HomingAccelerationMin = 1600.f;
	UPROPERTY(EditDefaultsOnly)
	float HomingAccelerationMax = 3200.f;
	UPROPERTY(EditDefaultsOnly)
	bool bLaunchHomingProjectiles = true;

private:
	UFUNCTION(BlueprintPure)
	static TArray<FRotator> EvenlySpacedRotators(const FVector& ForwardVector, const FVector& Axis, float Spread, int32 NumOfRotators);

	UFUNCTION(BlueprintPure)
	static TArray<FVector> EvenlyRotatedVectors(const FVector& ForwardVector, const FVector& Axis, float Spread, int32 NumOfVectors);
};
