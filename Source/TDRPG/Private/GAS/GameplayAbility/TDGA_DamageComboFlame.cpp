#include "GAS/GameplayAbility/TDGA_DamageComboFlame.h"
#include "Interface/ICombat.h"
#include "Actor/TDProjectile.h"
#include "GAS/AbilityTask/TDAT_TargetData.h"

void UTDGA_DamageComboFlame::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);

	FVector_NetQuantize MouseLocation = TargetDataAbilityTask->MouseCursorLocation;
	if (CurrentCombo == 1) SpawnProjectile(MouseLocation, SocketTag1);
	else if (CurrentCombo == 2) SpawnProjectile(MouseLocation, SocketTag2);
	else if (CurrentCombo == 3) SpawnProjectile(MouseLocation, SocketTag3);
	else if (CurrentCombo == 4) SpawnProjectile(MouseLocation, SocketTag4);
}

void UTDGA_DamageComboFlame::SpawnProjectile(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, bool bOverridePitch, float PitchOverride)
{
	// Projectile Actor를 서버에만 스폰시킨다.
	// 서버가 스폰시킨 Projectile Actor는 replicated actor가 되어 클라이언트는 replicated된 버젼을 보게 된다.
	if (false == GetAvatarActorFromActorInfo()->HasAuthority()) return; // 서버가 아닌 경우 예외처리

	IICombat* CombatInterface = Cast<IICombat>(GetAvatarActorFromActorInfo());
	if (CombatInterface)
	{
		//const FVector ForwardLocation = GetAvatarActorFromActorInfo()->GetActorForwardVector() * 700.f;
		const FVector SocketLocation = CombatInterface->GetCombatSocketLocationCPP(SocketTag); // Weapon, LeftHand, RightHand Socket 중 맞는것을 가져와서 사용.
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
