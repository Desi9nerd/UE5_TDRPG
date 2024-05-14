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

	IICombat* CombatInterface = Cast<IICombat>(OwnerCharacter); // �ڱ� �ڽ�.
	if (CombatInterface)
	{
		if (USkeletalMeshComponent* Weapon = CombatInterface->GetWeapon())
		{
			FHitResult HitResult;
			TArray<AActor*> ActorsToIgnore;
			ActorsToIgnore.Add(OwnerCharacter); // �ڱ��ڽ� ����.

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

	//RegisterOnDeathDelegate(MouseHitActor); // ���콺�� ����Ų ��(ĳ����)�� FOnDeathSignature ��������Ʈ ���.
	if (IICombat* CombatInterface2 = Cast<IICombat>(MouseHitActor)) // ���콺�� ����Ų ��(ĳ����).
	{
		if (false == CombatInterface2->GetOnDeathDelegate().IsAlreadyBound(this, &ThisClass::PrimaryTargetDiedBP))
		{
			CombatInterface2->GetOnDeathDelegate().AddDynamic(this, &ThisClass::PrimaryTargetDiedBP);
		}
	}
}

void UTDGA_DamageEnergyLance::StoreAdditionalTargets(TArray<AActor*>& OutAdditionalTargets)
{
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetAvatarActorFromActorInfo()); // �ڱ��ڽ� ����.
	ActorsToIgnore.Add(MouseHitActor); // ù��° Ÿ���� �̹� TraceFirstTarget���� Ÿ���õǾ����� ����.

	TArray<AActor*> OverlappingActors;
	UTDAbilitySystemBPLibrary::GetLivePlayersWithinRadius(
		GetAvatarActorFromActorInfo(),
		OverlappingActors,
		ActorsToIgnore,
		AdditionalTargetAcceptRadius,
		MouseHitActor->GetActorLocation());

	int32 NumAdditionalTargets = FMath::Min(GetAbilityLevel() - 1, MaxNumShockTargets);
	//int32 NumAdditionTargets = 5;

	UTDAbilitySystemBPLibrary::GetClosestTargets(5, OverlappingActors, OutAdditionalTargets, MouseHitActor->GetActorLocation());

	for (AActor* Target : OutAdditionalTargets)
	{
		//RegisterOnDeathDelegate(Target); // �ֺ��� ��ų ����Ǵ� �� �ϳ��ϳ� FOnDeathSignature ��������Ʈ ���.
		if (IICombat* CombatInterface = Cast<IICombat>(Target))
		{
			if (false == CombatInterface->GetOnDeathDelegate().IsAlreadyBound(this, &ThisClass::AdditionalTargetDiedBP))
			{
				CombatInterface->GetOnDeathDelegate().AddDynamic(this, &ThisClass::AdditionalTargetDiedBP);
			}
		}
	}
}

void UTDGA_DamageEnergyLance::RegisterOnDeathDelegate(AActor* InActor)
{
	// InActor(= ���콺�� ����Ų ��, �ֺ��� ��ų ����Ǵ� ��)�� FOnDeathSignature ��������Ʈ ���.
	IICombat* CombatInterface = Cast<IICombat>(InActor);
	if (CombatInterface)
	{
		if (false == CombatInterface->GetOnDeathDelegate().IsAlreadyBound(this, &ThisClass::PrimaryTargetDied))
		{
			CombatInterface->GetOnDeathDelegate().AddDynamic(this, &ThisClass::PrimaryTargetDied);
		}
	}
}

void UTDGA_DamageEnergyLance::PrimaryTargetDied(AActor* DeadActor)
{
}

void UTDGA_DamageEnergyLance::AdditionalTargetDied(AActor* DeadActor)
{
}
