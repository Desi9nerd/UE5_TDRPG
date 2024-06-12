#include "GAS/GameplayAbility/TDGA_DamageComboFlame.h"
#include "Interface/ICombat.h"
#include "Actor/TDProjectile.h"
#include "Actor/TDProjectileFlame.h"
#include "GAS/TDAbilitySystemBPLibrary.h"
#include "GAS/AbilityTask/TDAT_TargetData.h"

void UTDGA_DamageComboFlame::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);
	
	//FVector_NetQuantize MouseLocation = TargetDataAbilityTask->MouseCursorLocation;
	//if (CurrentCombo == 1) SpawnProjectile(MouseLocation, SocketTag1);
	//else if (CurrentCombo == 2) SpawnProjectile(MouseLocation, SocketTag2);
	//else if (CurrentCombo == 3) SpawnProjectile(MouseLocation, SocketTag3);
	//else if (CurrentCombo == 4) SpawnProjectile(MouseLocation, SocketTag4);
}

TArray<ATDProjectileFlame*> UTDGA_DamageComboFlame::SpawnFlames()
{
	TArray<ATDProjectileFlame*> Flames;
	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	TArray<FRotator> Rotators = EvenlySpacedRotators(Forward, FVector::UpVector, 120.f, NumOfProjectiles);

	for (const FRotator& Rotator : Rotators)
	{
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(Location);
		SpawnTransform.SetRotation(Rotator.Quaternion());

		ATDProjectileFlame* FlameProjectile = GetWorld()->SpawnActorDeferred<ATDProjectileFlame>(ProjectileClass, SpawnTransform, GetOwningActorFromActorInfo(), CurrentActorInfo->PlayerController->GetPawn(), ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		FlameProjectile->DamageEffectParams = SetDamageEffectParams();
		FlameProjectile->ReturnToActor = GetAvatarActorFromActorInfo();

		FlameProjectile->ExplosionDamageParams = SetDamageEffectParams();
		FlameProjectile->SetOwner(GetAvatarActorFromActorInfo());

		Flames.Add(FlameProjectile);

		FlameProjectile->FinishSpawning(SpawnTransform);
	}

	return Flames;
}

void UTDGA_DamageComboFlame::SpawnProjectile(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, bool bOverridePitch, float PitchOverride)
{
	// Projectile Actor�� �������� ������Ų��.
	// ������ ������Ų Projectile Actor�� replicated actor�� �Ǿ� Ŭ���̾�Ʈ�� replicated�� ������ ���� �ȴ�.
	if (false == GetAvatarActorFromActorInfo()->HasAuthority()) return; // ������ �ƴ� ��� ����ó��

	IICombat* CombatInterface = Cast<IICombat>(GetAvatarActorFromActorInfo());
	if (CombatInterface)
	{
		//const FVector ForwardLocation = GetAvatarActorFromActorInfo()->GetActorForwardVector() * 700.f;
		const FVector SocketLocation = CombatInterface->GetCombatSocketLocationCPP(SocketTag); // Weapon, LeftHand, RightHand Socket �� �´°��� �����ͼ� ���.
		FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();

		if (bOverridePitch)
		{
			Rotation.Pitch = PitchOverride;
		}

		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		SpawnTransform.SetRotation(Rotation.Quaternion());

		ATDProjectile* Projectile = GetWorld()->SpawnActorDeferred<ATDProjectile>(ProjectileClass, SpawnTransform, GetOwningActorFromActorInfo(), Cast<APawn>(GetOwningActorFromActorInfo()), ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		
		
		Projectile->DamageEffectParams = SetDamageEffectParams();

		Projectile->FinishSpawning(SpawnTransform);
	}
}

TArray<FRotator> UTDGA_DamageComboFlame::EvenlySpacedRotators(const FVector& ForwardVector, const FVector& Axis, float Spread, int32 NumOfRotators)
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
