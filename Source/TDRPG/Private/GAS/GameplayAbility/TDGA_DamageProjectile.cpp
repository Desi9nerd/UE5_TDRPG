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

	//IICombat* CombatInterface = Cast<IICombat>(GetAvatarActorFromActorInfo());
	//if (CombatInterface)
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

		const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(GameplayEffectDamageClass, GetAbilityLevel(), EffectContextHandle);


		const FTDGameplayTags GameplayTags = FTDGameplayTags::GetTDGameplayTags();

		for (auto& Pair : DamageTypes)
		{
			const float ScaledDamage = Pair.Value.GetValueAtLevel(GetAbilityLevel());
			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, Pair.Key, ScaledDamage);// GameplayEffect에서 Set by Caller로 사용할 때 적용되는 key, value. FTDGameplayTags구조체 내의 Damage타입의 GameplayTag를 key로, ScaledDamage을 value로 사용.
		}

		Projectile->DamageEffectSpecHandle = SpecHandle;

		Projectile->FinishSpawning(SpawnTransform);
	}
}

FString UTDGA_DamageProjectile::GetDescription(int32 AbilityLevel)
{
	const int32 Damage = DamageTypes[FTDGameplayTags::GetTDGameplayTags().Damage_Fireball].GetValueAtLevel(AbilityLevel);
	if (AbilityLevel == 1)
	{
		return FString::Printf(TEXT("<Title>파이어볼</>\n\n<Default> 파이어볼을 날려 적에게 피해를 입힙니다. 데미지 피해: </><Damage>%d</>\n\n<Small>Level: </><Level>%d</>"), Damage, AbilityLevel);
	}

	return FString::Printf(TEXT("<Title>파이어볼</>\n\n<Default> %d 개의 파이어볼을 날려 적에게 피해를 입힙니다. \n 데미지 피해: </><Damage>%d</>\n\n<Small>스킬레벨: </><Level>%d</>"), FMath::Min(AbilityLevel, NumProjectiles), Damage, AbilityLevel);
}

FString UTDGA_DamageProjectile::GetNextAbilityLevelDescription(int32 AbilityLevel)
{
	const int32 Damage = DamageTypes[FTDGameplayTags::GetTDGameplayTags().Damage_Fireball].GetValueAtLevel(AbilityLevel);

	return FString::Printf(TEXT("<Title>다음 스킬레벨: </>\n\n<Default> %d 개의 파이어볼을 날려 적에게 피해를 입힙니다. \n 데미지 피해: </><Damage>%d</>\n\n<Small>스킬레벨: </><Level>%d</>"), FMath::Min(AbilityLevel, NumProjectiles), Damage, AbilityLevel);
}