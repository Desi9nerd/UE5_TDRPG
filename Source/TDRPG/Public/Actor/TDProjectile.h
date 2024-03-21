

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TDProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class TDRPG_API ATDProjectile : public AActor
{
	GENERATED_BODY()
	
public:
	ATDProjectile();

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Sphere;

};
