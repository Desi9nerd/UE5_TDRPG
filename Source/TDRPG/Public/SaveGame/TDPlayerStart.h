#pragma once
#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "Interface/ISaveGame.h"
#include "TDPlayerStart.generated.h"

class USphereComponent;
class UWidgetComponent;

/**
 * 
 */

UCLASS()
class TDRPG_API ATDPlayerStart : public APlayerStart, public IISaveGame
{
	GENERATED_BODY()

public:
	ATDPlayerStart(const FObjectInitializer& ObjectInitializer);

	//** IISaveGame
	virtual bool ShouldLoadTransform() override { return false; }
	virtual void LoadActor() override;
	//*****************************************************

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintImplementableEvent)
	void CheckpointReached(UMaterialInstanceDynamic* DynamicMaterialInstance);

	void HandleGlowEffects();

	UPROPERTY(BlueprintReadOnly, SaveGame)
	bool bReached = false;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> CheckpointMesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Sphere;
};

/* 참고) UPROPERTY(SaveGame) 란?
 * Variables marked with SaveGame will be serialized by a USaveGame object.
 * This Specifier is a simple way to include fields explicitly for a checkpoint/save system at the property level. The flag should be set on all fields that are intended to be part of a saved game, and then a proxy archiver can be used to read/write it.
 */
