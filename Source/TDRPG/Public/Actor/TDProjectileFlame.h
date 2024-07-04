#pragma once
#include "CoreMinimal.h"
#include "Actor/TDProjectile.h"
#include "TDProjectileFlame.generated.h"

/**
 * 
 */
UCLASS()
class TDRPG_API ATDProjectileFlame : public ATDProjectile
{
	GENERATED_BODY()

public:
	ATDProjectileFlame();

	UFUNCTION(BlueprintCallable)
	bool IsWithinExplodeDistance() const;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AActor> ReturnToActor;

	UPROPERTY(BlueprintReadWrite)
	FDamageEffectParams ExplosionDamageParams;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

private:
	void StoreInitialandApexLocations();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	FVector InitialLocation;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	FVector ApexLocation;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	FVector TargetLocation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	float TravelDistance = 600.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	float ExplodeDistance;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	float Alpha;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	float RadialDamageOuterRadius;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	float RadialDamageInnerRadius;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	float RagdollImpulseMagnitude;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	float KnockbackMagnitude;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	FVector KnockbackDirection;
};
