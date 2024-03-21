#include "GAS/GameplayAbility/TDGAProjectile.h"
#include "Actor/TDProjectile.h"
#include "Interface/ICombat.h"

void UTDGAProjectile::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// Projectile Actor를 서버에만 스폰시킨다.
	// 서버가 스폰시킨 Projectile Actor는 replicated actor가 되어 클라이언트는 replicated된 버젼을 보게 된다.
	if (false == HasAuthority(&ActivationInfo)) return; // 서버가 아닌 경우 예외처리

	IICombat* CombatInterface = Cast<IICombat>(GetAvatarActorFromActorInfo());
	if (CombatInterface)
	{
		const FVector SocketLocation = CombatInterface->GetCombatSocketLocation();

		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);

		// TODO: Projectile Rotation 설정하기.

		ATDProjectile* Projectile = GetWorld()->SpawnActorDeferred<ATDProjectile>(ProjectileClass, SpawnTransform, GetOwningActorFromActorInfo(), Cast<APawn>(GetOwningActorFromActorInfo()), ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		// TODO: Gameplay Effect Spec으로 데미지 처리.

		Projectile->FinishSpawning(SpawnTransform);
	}

}
