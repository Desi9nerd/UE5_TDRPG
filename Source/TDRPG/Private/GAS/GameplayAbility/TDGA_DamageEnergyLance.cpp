#include "GAS/GameplayAbility/TDGA_DamageEnergyLance.h"
#include "GameFramework/Character.h"
#include "GAS/TDAbilitySystemBPLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

void UTDGA_DamageEnergyLance::StoreMouseDataInfo(const FHitResult& HitResult)
{
	if (HitResult.bBlockingHit)
	{
		MouseHitLocation = HitResult.ImpactPoint;
		MouseHitActor = HitResult.GetActor();
	}
	else
	{
		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
	}
}

void UTDGA_DamageEnergyLance::StoreOwnerVariables()
{
	if (CurrentActorInfo)
	{
		OwnerPlayerController = CurrentActorInfo->PlayerController.Get();
		OwnerCharacter = Cast<ACharacter>(CurrentActorInfo->AvatarActor);
	}
}

void UTDGA_DamageEnergyLance::TraceFirstTarget(const FVector& BeamTargetLocation)
{
	checkf(OwnerCharacter, TEXT("No OwnerCharacter, Check UTDGA_DamageEnergyLance::TraceFirstTarget"));

	IICombat* CombatInterface = Cast<IICombat>(OwnerCharacter);
	if (CombatInterface)
	{
		if (USkeletalMeshComponent* Weapon = CombatInterface->GetWeapon())
		{
			FHitResult HitResult;
			TArray<AActor*> ActorsToIgnore;
			ActorsToIgnore.Add(OwnerCharacter); // 자기자신 제외.

			UKismetSystemLibrary::SphereTraceSingle(
				OwnerCharacter,
				Weapon->GetSocketLocation(FName("TipSocket")),
				BeamTargetLocation,
				10.f,
				TraceTypeQuery1,
				false,
				ActorsToIgnore,
				EDrawDebugTrace::None,
				HitResult,
				true);

			if (HitResult.bBlockingHit)
			{
				MouseHitLocation = HitResult.ImpactPoint;
				MouseHitActor = HitResult.GetActor();
			}
		}
	}
}

void UTDGA_DamageEnergyLance::StoreAdditionalTargets(TArray<AActor*>& OutAdditionalTargets)
{
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetAvatarActorFromActorInfo()); // 자기자신 제외.
	ActorsToIgnore.Add(MouseHitActor); // 첫번째 타겟은 이미 TraceFirstTarget에서 타겟팅되었으니 제외.

	TArray<AActor*> OverlappingActors;
	UTDAbilitySystemBPLibrary::GetLivePlayersWithinRadius(
		GetAvatarActorFromActorInfo(),
		OverlappingActors,
		ActorsToIgnore,
		AdditionalTargetAcceptRadius,
		MouseHitActor->GetActorLocation());

	//int32 NumAdditionalTargets = FMath::Min(GetAbilityLevel() - 1, MaxNumShockTargets);
	int32 NumAdditionTargets = 5;

	UTDAbilitySystemBPLibrary::GetClosestTargets(NumAdditionTargets, OverlappingActors, OutAdditionalTargets, MouseHitActor->GetActorLocation());
}
