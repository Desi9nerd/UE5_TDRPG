#include "Actor/TDProjectileFlame.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/TimelineComponent.h"
#include "GAS/TDAbilitySystemBPLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

ATDProjectileFlame::ATDProjectileFlame()
{
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMovement->InitialSpeed = 0.f;
	ProjectileMovement->MaxSpeed = 0.f;
	ProjectileMovement->bAutoActivate = false;

	//OutgoingTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("OutgoingTimeline"));
	//ReturningTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("ReturningTimeline"));
}

void ATDProjectileFlame::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		StoreInitialandApexLocations();
	}
}

void ATDProjectileFlame::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ATDProjectileFlame::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (false == IsValidOverlap(OtherActor)) return;

	if (HasAuthority())
	{
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			const FVector RagdollImpulse = GetActorForwardVector() * DamageEffectParams.RagdollImpulseMagnitude;
			DamageEffectParams.RagdollImpulse = RagdollImpulse;

			DamageEffectParams.TargetAbilitySystemComponent = TargetASC;
			UTDAbilitySystemBPLibrary::ApplyDamageEffect(DamageEffectParams);
		}
	}
}

void ATDProjectileFlame::StoreInitialandApexLocations()
{
	InitialLocation = GetActorLocation();
	ApexLocation = InitialLocation + GetActorLocation() * TravelDistance;
}

bool ATDProjectileFlame::IsWithinExplodeDistance() const
{
	return FVector::Dist(GetActorLocation(), ReturnToActor->GetActorLocation()) < ExplodeDistance;
}




//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
void ATDProjectileFlame::StartOutgoingTimeline()
{
	InitialLocation = GetActorLocation();
	ApexLocation = InitialLocation + GetActorForwardVector() * TravelDistance;

	OutgoingTimeline->PlayFromStart();

	if (Curve1 != nullptr)
	{
		FOnTimelineFloat Curve1Callback;
		FOnTimelineVector Curve2Callback;
		FOnTimelineLinearColor Curve3Callback;

		FOnTimelineEvent LerpTimelineFinishedCallback;

		Curve1Callback.BindUFunction(this, FName());
		Curve2Callback.BindUFunction(this, FName());
		Curve3Callback.BindUFunction(this, FName());
		LerpTimelineFinishedCallback.BindUFunction(this, FName(TEXT("OnLerpTimelineFinished")));

		//LerpTimeline.AddInterpFloat(Curve1, Curve1Callback);
		//LerpTimeline.AddInterpVector(Curve2, Curve2Callback);
		//LerpTimeline.AddInterpLinearColor(Curve3, Curve3Callback);

		LerpTimeline.SetTimelinePostUpdateFunc()
		LerpTimeline.SetTimelineFinishedFunc(LerpTimelineFinishedCallback); // Timeline이 끝날 때 호출될 callback 함수 설정.

		LerpTimeline.SetTimelineLength(LerpTimelineLength); // Timeline 길이 설정.
	}
	LerpTimeline.PlayFromStart(); // Timeline을 처음부터 재생.

}

void ATDProjectileFlame::UpdateOutgoingTimeline()
{
	SetActorLocation(FMath::Lerp(InitialLocation, ApexLocation, Alpha));
}

void ATDProjectileFlame::UpdateReturningTimeline()
{
	SetActorLocation(FMath::Lerp(ApexLocation, InitialLocation, Alpha));
}

void ATDProjectileFlame::OnOutgoingTimelineFinished()
{
	if (IsWithinExplodeDistance())
	{
		// Handle explosion logic
	}
	else
	{
		ReturningTimeline->PlayFromStart();
	}
}

void ATDProjectileFlame::OnReturningTimelineFinished()
{
	if (IsWithinExplodeDistance())
	{
		Destroyed();
		//UTDAbilitySystemBPLibrary::SetIsRadialDamage(ExplosionDamageParams, true);
	}
	else
	{
		StartOutgoingTimeline();
	}
}
*/
