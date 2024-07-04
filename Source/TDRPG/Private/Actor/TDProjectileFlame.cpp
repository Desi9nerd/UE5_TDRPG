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
