#include "GAS/GameplayAbility/TDGAProjectile.h"
#include "Actor/TDProjectile.h"
#include "Interface/ICombat.h"

void UTDGAProjectile::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// Projectile Actor�� �������� ������Ų��.
	// ������ ������Ų Projectile Actor�� replicated actor�� �Ǿ� Ŭ���̾�Ʈ�� replicated�� ������ ���� �ȴ�.
	if (false == HasAuthority(&ActivationInfo)) return; // ������ �ƴ� ��� ����ó��

	IICombat* CombatInterface = Cast<IICombat>(GetAvatarActorFromActorInfo());
	if (CombatInterface)
	{
		const FVector SocketLocation = CombatInterface->GetCombatSocketLocation();

		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);

		// TODO: Projectile Rotation �����ϱ�.

		ATDProjectile* Projectile = GetWorld()->SpawnActorDeferred<ATDProjectile>(ProjectileClass, SpawnTransform, GetOwningActorFromActorInfo(), Cast<APawn>(GetOwningActorFromActorInfo()), ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		// TODO: Gameplay Effect Spec���� ������ ó��.

		Projectile->FinishSpawning(SpawnTransform);
	}

}
