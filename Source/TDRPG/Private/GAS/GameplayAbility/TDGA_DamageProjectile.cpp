#include "GAS/GameplayAbility/TDGA_DamageProjectile.h"
#include "Actor/TDProjectile.h"
#include "Interface/ICombat.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayTags/TDGameplayTags.h"

void UTDGA_DamageProjectile::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	//SpawnProjectile();
}

//void UTDGA_Projectile::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
//	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
//{
//	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
//}

void UTDGA_DamageProjectile::SpawnProjectile(const FVector& ProjectileTargetLocation)
{
	// Projectile Actor�� �������� ������Ų��.
	// ������ ������Ų Projectile Actor�� replicated actor�� �Ǿ� Ŭ���̾�Ʈ�� replicated�� ������ ���� �ȴ�.
	if (false == GetAvatarActorFromActorInfo()->HasAuthority()) return; // ������ �ƴ� ��� ����ó��

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

		// Gameplay Effect Spec���� ������ ó��.
		const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
		FGameplayEffectContextHandle EffectContextHandle = SourceASC->MakeEffectContext();
		EffectContextHandle.SetAbility(this); // AbilityInstanceNotReplicated/CDO/Level ����
		EffectContextHandle.AddSourceObject(Projectile);
		TArray<TWeakObjectPtr<AActor>> Actors;
		Actors.Add(Projectile);
		EffectContextHandle.AddActors(Actors);
		FHitResult HitResult;
		HitResult.Location = ProjectileTargetLocation;
		EffectContextHandle.AddHitResult(HitResult);

		const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(GameplayEffectDamageClass, GetAbilityLevel(), EffectContextHandle);


		FTDGameplayTags GameplayTags = FTDGameplayTags::GetTDGameplayTags();

		for (auto& Pair : DamageTypes)
		{
			const float ScaledDamage = Pair.Value.GetValueAtLevel(GetAbilityLevel());
			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, Pair.Key, ScaledDamage);// GameplayEffect���� Set by Caller�� ����� �� ����Ǵ� key, value. FTDGameplayTags����ü ���� DamageŸ���� GameplayTag�� key��, ScaledDamage�� value�� ���.
		}

		Projectile->DamageEffectSpecHandle = SpecHandle;

		Projectile->FinishSpawning(SpawnTransform);
	}
}
