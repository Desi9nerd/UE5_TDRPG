#include "GAS/GameplayAbility/TDGA_DamageProjectile_Arrow.h"
#include "Interface/ICombat.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Actor/TDProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"

FString UTDGA_DamageProjectile_Arrow::GetDescription(int32 Level)
{
	return Super::GetDescription(Level);
}

FString UTDGA_DamageProjectile_Arrow::GetNextAbilityLevelDescription(int32 AbilityLevel)
{
	return Super::GetNextAbilityLevelDescription(AbilityLevel);
}

void UTDGA_DamageProjectile_Arrow::SpawnProjectiles(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, bool bOverridePitch, float PitchOverride, AActor* HomingTarget)
{
	if (false == GetAvatarActorFromActorInfo()->HasAuthority()) return;

	IICombat* CombatInterface = Cast<IICombat>(GetAvatarActorFromActorInfo());

	const FVector SocketLocation = CombatInterface->GetCombatSocketLocationCPP(SocketTag);
	FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
	if (bOverridePitch) // 발사체의 Pitch(위아래) 여부 확인 후 적용
	{
		if (Cast<IICombat>(HomingTarget))
		{
			Rotation.Pitch = PitchOverride + 5.f;
		}
		else
		{
			Rotation.Pitch = PitchOverride;
		}
	}


	const FVector Forward = Rotation.Vector();

	const int32 EffectiveNumProjectiles = FMath::Min(NumProjectiles, GetAbilityLevel());
	TArray<FRotator> Rotations = EvenlySpacedRotators(Forward, FVector::UpVector, ArrowSpread, NumOfArrowsForTesting);
	//TArray<FRotator> Rotations = EvenlySpacedRotators(Forward, FVector::UpVector, ArrowSpread, EffectiveNumProjectiles);

	for (const FRotator& Rot : Rotations)
	{
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		SpawnTransform.SetRotation(Rot.Quaternion());

		ATDProjectile* Projectile = GetWorld()->SpawnActorDeferred<ATDProjectile>(ProjectileClass, SpawnTransform, GetOwningActorFromActorInfo(), Cast<APawn>(GetOwningActorFromActorInfo()), ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		Projectile->DamageEffectParams = SetDamageEffectParams();

		if (HomingTarget && Cast<IICombat>(HomingTarget))
		{
			Projectile->ProjectileMovement->HomingTargetComponent = HomingTarget->GetRootComponent();
		}
		//else
		//{
		//	Projectile->TargetSceneComponent = NewObject<USceneComponent>(USceneComponent::StaticClass());
		//	Projectile->TargetSceneComponent->SetWorldLocation(ProjectileTargetLocation);
		//	Projectile->ProjectileMovement->HomingTargetComponent = Projectile->TargetSceneComponent;
		//}

		Projectile->ProjectileMovement->HomingAccelerationMagnitude = FMath::FRandRange(HomingAccelerationMin, HomingAccelerationMax);
		Projectile->ProjectileMovement->bIsHomingProjectile = bLaunchHomingProjectiles;

		Projectile->FinishSpawning(SpawnTransform);
	}
}

TArray<FRotator> UTDGA_DamageProjectile_Arrow::EvenlySpacedRotators(const FVector& ForwardVector, const FVector& Axis, float Spread, int32 NumOfRotators)
{
	TArray<FRotator> Rotators;

	if (NumOfRotators > 1)
	{
		const float DeltaSpread = Spread / (NumOfRotators - 1);
		for (int32 i = 0; i < NumOfRotators; i++)
		{
			const FVector LeftOfSpread = ForwardVector.RotateAngleAxis(-Spread / 2.f, Axis);
			const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
			Rotators.Add(Direction.Rotation());
		}
	}
	else
	{
		Rotators.Add(ForwardVector.Rotation());
	}

	return Rotators;
}

TArray<FVector> UTDGA_DamageProjectile_Arrow::EvenlyRotatedVectors(const FVector& ForwardVector, const FVector& Axis, float Spread, int32 NumOfVectors)
{
	TArray<FVector> Vectors;

	if (NumOfVectors > 1)
	{
		const float DeltaSpread = Spread / (NumOfVectors - 1);
		for (int32 i = 0; i < NumOfVectors; i++)
		{
			const FVector LeftOfSpread = ForwardVector.RotateAngleAxis(-Spread / 2.f, Axis);
			const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
			Vectors.Add(Direction);
		}
	}
	else
	{
		Vectors.Add(ForwardVector);
	}
	return Vectors;
}
