#include "Actor/TDFrozenFieldActor.h"
#include "GAS/TDAbilitySystemBPLibrary.h"
#include "Kismet/KismetMathLibrary.h"

ATDFrozenFieldActor::ATDFrozenFieldActor()
{
	PrimaryActorTick.bCanEverTick = false;

	FrozenField = CreateDefaultSubobject<USceneComponent>(TEXT("FrozenField"));
	SetRootComponent(FrozenField);
}

USceneComponent* ATDFrozenFieldActor::GetGroundPoint(const FVector& GroundLocation, int32 NumPoints, float YawOverride)
{
	checkf(FrozenField, TEXT("No FrozenField. Check ATDFrozenFieldActor::GetGroundPoint"));

	USceneComponent* Point = nullptr;

	const FVector RaisedLocation = FVector(Point->GetComponentLocation().X, Point->GetComponentLocation().Y, Point->GetComponentLocation().Z + 500.f);
	const FVector LoweredLocation = FVector(Point->GetComponentLocation().X, Point->GetComponentLocation().Y, Point->GetComponentLocation().Z - 500.f);

	FHitResult HitResult;
	TArray<AActor*> IgnoreActors;
	UTDAbilitySystemBPLibrary::GetLivePlayersWithinRadius(this, IgnoreActors, TArray<AActor*>(), 1500.f, GetActorLocation());

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActors(IgnoreActors);
	GetWorld()->LineTraceSingleByProfile(HitResult, RaisedLocation, LoweredLocation, FName(TEXT("BlockAll")), QueryParams);

	const FVector AdjustedLocation = FVector(Point->GetComponentLocation().X, Point->GetComponentLocation().Y, HitResult.ImpactPoint.Z);
	Point->SetWorldLocation(AdjustedLocation);
	Point->SetWorldRotation(UKismetMathLibrary::MakeRotFromZ(HitResult.ImpactNormal));


	return Point;
}
