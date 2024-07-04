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

void UTDGA_DamageProjectile::SpawnProjectile(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, bool bOverridePitch, float PitchOverride)
{
	// Projectile Actor를 서버에만 스폰시킨다.
	// 서버가 스폰시킨 Projectile Actor는 replicated actor가 되어 클라이언트는 replicated된 버젼을 보게 된다.
	if (false == GetAvatarActorFromActorInfo()->HasAuthority()) return; // 서버가 아닌 경우 예외처리

	IICombat* CombatInterface = Cast<IICombat>(GetAvatarActorFromActorInfo());
	if (CombatInterface)
	{
		const FVector SocketLocation = IICombat::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(),
			SocketTag); // Weapon, LeftHand, RightHand Socket 중 맞는것을 가져와서 사용.
		//const FVector SocketLocation = CombatInterface->GetCombatSocketLocation();
		FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();

		if (bOverridePitch)
		{
			Rotation.Pitch = PitchOverride;
		}

		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		SpawnTransform.SetRotation(Rotation.Quaternion());

		ATDProjectile* Projectile = GetWorld()->SpawnActorDeferred<ATDProjectile>(ProjectileClass, SpawnTransform, GetOwningActorFromActorInfo(), Cast<APawn>(GetOwningActorFromActorInfo()), ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		/*
		// 방법1: Gameplay Effect Spec
		// Gameplay Effect Spec으로 데미지 처리.
		const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
		FGameplayEffectContextHandle EffectContextHandle = SourceASC->MakeEffectContext();
		EffectContextHandle.SetAbility(this); // AbilityInstanceNotReplicated/CDO/Level 세팅
		EffectContextHandle.AddSourceObject(Projectile);
		TArray<TWeakObjectPtr<AActor>> Actors;
		Actors.Add(Projectile);
		EffectContextHandle.AddActors(Actors);
		FHitResult HitResult;
		HitResult.Location = ProjectileTargetLocation;
		EffectContextHandle.AddHitResult(HitResult);

		const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(GEDamageClass, GetAbilityLevel(), EffectContextHandle);

		const FTDGameplayTags GameplayTags = FTDGameplayTags::GetTDGameplayTags();

		const float ScaledDamage = DamageScalableFloat.GetValueAtLevel(GetAbilityLevel());
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, DamageType, ScaledDamage);// GameplayEffect에서 Set by Caller로 사용할 때 적용되는 key, value. FTDGameplayTags구조체 내의 Damage타입의 GameplayTag를 key로, ScaledDamage을 value로 사용.

		Projectile->DamageEffectSpecHandle = SpecHandle; // Projectile 액터의 DamageEffectSpecHandle에 값을 넣어줌.
		*/

		// 방법2: Damage Effect Params
		Projectile->DamageEffectParams = SetDamageEffectParams();
		//Projectile->DamageEffectParams.KnockbackImpulse = FVector(600.f, 600.f, 0.f);

		// TODO : Knockback
		//UE_LOG(LogTemp, Warning, TEXT(" = %f"), Projectile->DamageEffectParams.TargetAbilitySystemComponent);
		//UE_LOG(LogTemp, Warning, TEXT("Knockback Chance = %f"), Projectile->DamageEffectParams.KnockbackChance);
		//UE_LOG(LogTemp, Warning, TEXT("Knockback Impulse Magnitude = %f"), Projectile->DamageEffectParams.KnockbackImpulseMagnitude);
		//UE_LOG(LogTemp, Warning, TEXT("Knockback Impulse FVector = %s"), *Projectile->DamageEffectParams.KnockbackImpulse.ToString());

		Projectile->FinishSpawning(SpawnTransform);
	}
}
