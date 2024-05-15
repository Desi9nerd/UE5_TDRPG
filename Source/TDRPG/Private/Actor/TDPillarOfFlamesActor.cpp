#include "Actor/TDPillarOfFlamesActor.h"

#include "GAS/TDAbilitySystemBPLibrary.h"
#include "Kismet/KismetMathLibrary.h"

ATDPillarOfFlamesActor::ATDPillarOfFlamesActor()
{
	PrimaryActorTick.bCanEverTick = true;

	OriginPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Point"));
	Point_0 = CreateDefaultSubobject<USceneComponent>(TEXT("Point_0"));
	Point_1 = CreateDefaultSubobject<USceneComponent>(TEXT("Point_1"));
	Point_2 = CreateDefaultSubobject<USceneComponent>(TEXT("Point_2"));
	Point_3 = CreateDefaultSubobject<USceneComponent>(TEXT("Point_3"));
	Point_4 = CreateDefaultSubobject<USceneComponent>(TEXT("Point_4"));
	Point_5 = CreateDefaultSubobject<USceneComponent>(TEXT("Point_5"));
	Point_6 = CreateDefaultSubobject<USceneComponent>(TEXT("Point_6"));

	SetRootComponent(OriginPoint);
	Point_0->SetupAttachment(OriginPoint);
	Point_1->SetupAttachment(OriginPoint);
	Point_2->SetupAttachment(OriginPoint);
	Point_3->SetupAttachment(OriginPoint);
	Point_4->SetupAttachment(OriginPoint);
	Point_5->SetupAttachment(OriginPoint);
	Point_6->SetupAttachment(OriginPoint);

	ImmutablePoints.Add(Point_0);
	ImmutablePoints.Add(Point_1);
	ImmutablePoints.Add(Point_2);
	ImmutablePoints.Add(Point_3);
	ImmutablePoints.Add(Point_4);
	ImmutablePoints.Add(Point_5);
	ImmutablePoints.Add(Point_6);
}

TArray<USceneComponent*> ATDPillarOfFlamesActor::GetSpawnPoints(const FVector& GroundLocation, int32 NumPoints, float YawOverride)
{
	checkf(ImmutablePoints.Num() >= NumPoints, TEXT("Out Of Bounds. Check ATDPillarOfFlamesActor::GetSpawnPoints"));

	TArray<USceneComponent*> ArrayCopy;

	for (USceneComponent* Pt : ImmutablePoints)
	{
		if (ArrayCopy.Num() >= NumPoints) return ArrayCopy;

		if (Pt != Point_0)
		{
			FVector ToPoint = Pt->GetComponentLocation() - Point_0->GetComponentLocation();
			ToPoint = ToPoint.RotateAngleAxis(YawOverride, FVector::UpVector);
			Pt->SetWorldLocation(Point_0->GetComponentLocation() + ToPoint);
		}

		const FVector RaisedLocation = FVector(Pt->GetComponentLocation().X, Pt->GetComponentLocation().Y, Pt->GetComponentLocation().Z + 500.f);
		const FVector LoweredLocation = FVector(Pt->GetComponentLocation().X, Pt->GetComponentLocation().Y, Pt->GetComponentLocation().Z - 500.f);

		FHitResult HitResult;
		TArray<AActor*> IgnoreActors;
		UTDAbilitySystemBPLibrary::GetLivePlayersWithinRadius(this, IgnoreActors, TArray<AActor*>(), SkillRadius, GetActorLocation());

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActors(IgnoreActors);
		GetWorld()->LineTraceSingleByProfile(HitResult, RaisedLocation, LoweredLocation, FName(TEXT("BlockAll")), QueryParams);

		const FVector AdjustedLocation = FVector(Pt->GetComponentLocation().X, Pt->GetComponentLocation().Y, HitResult.ImpactPoint.Z);
		Pt->SetWorldLocation(AdjustedLocation);
		Pt->SetWorldRotation(UKismetMathLibrary::MakeRotFromZ(HitResult.ImpactNormal));

		ArrayCopy.Add(Pt);
	}

	return ArrayCopy;
}
