#include "GAS/GameplayAbility/TDGA_Projectile.h"
#include "Actor/TDProjectile.h"
#include "Interface/ICombat.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

void UTDGA_Projectile::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	//SpawnProjectile();
}

//void UTDGA_Projectile::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
//	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
//{
//	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
//}

void UTDGA_Projectile::SpawnProjectile(const FVector& ProjectileTargetLocation)
{
	// Projectile Actor를 서버에만 스폰시킨다.
	// 서버가 스폰시킨 Projectile Actor는 replicated actor가 되어 클라이언트는 replicated된 버젼을 보게 된다.
	if (false == GetAvatarActorFromActorInfo()->HasAuthority()) return; // 서버가 아닌 경우 예외처리

	IICombat* CombatInterface = Cast<IICombat>(GetAvatarActorFromActorInfo());
	if (CombatInterface)
	{
		const FVector SocketLocation = CombatInterface->GetCombatSocketLocation();
		FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
		Rotation.Pitch = 0.f;

		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		SpawnTransform.SetRotation(Rotation.Quaternion());

		ATDProjectile* Projectile = GetWorld()->SpawnActorDeferred<ATDProjectile>(ProjectileClass, SpawnTransform, GetOwningActorFromActorInfo(), Cast<APawn>(GetOwningActorFromActorInfo()), ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		// Gameplay Effect Spec으로 데미지 처리.
		const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
		const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(GameplayEffectDamageClass, GetAbilityLevel(), SourceASC->MakeEffectContext());
		Projectile->DamageEffectSpecHandle = SpecHandle;

		Projectile->FinishSpawning(SpawnTransform);
	}
}
