#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TDPillarOfFlamesActor.generated.h"

UCLASS()
class TDRPG_API ATDPillarOfFlamesActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ATDPillarOfFlamesActor();

	UFUNCTION(BlueprintPure)
	TArray<USceneComponent*> GetSpawnPoints(const FVector& GroundLocation, int32 NumPoints, float YawOverride = 0.f);

private:
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess))
	float SkillRadius = 1500.f;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess))
	TArray<USceneComponent*> ImmutablePoints;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess))
	TObjectPtr<USceneComponent> OriginPoint;
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess))
	TObjectPtr<USceneComponent> Point_0;
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess))
	TObjectPtr<USceneComponent> Point_1;
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess))
	TObjectPtr<USceneComponent> Point_2;
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess))
	TObjectPtr<USceneComponent> Point_3;
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess))
	TObjectPtr<USceneComponent> Point_4;
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess))
	TObjectPtr<USceneComponent> Point_5;
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess))
	TObjectPtr<USceneComponent> Point_6;
};
