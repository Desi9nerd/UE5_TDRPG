#pragma once
#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "TDPlayerStart.generated.h"

class USphereComponent;

/**
 * 
 */

UCLASS()
class TDRPG_API ATDPlayerStart : public APlayerStart
{
	GENERATED_BODY()

public:
	ATDPlayerStart(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintImplementableEvent)
	void CheckpointReached(UMaterialInstanceDynamic* DynamicMaterialInstance);

	void HandleGlowEffects();

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> CheckpointMesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Sphere;
};
