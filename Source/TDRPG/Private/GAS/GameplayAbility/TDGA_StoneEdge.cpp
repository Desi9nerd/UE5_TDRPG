#include "GAS/GameplayAbility/TDGA_StoneEdge.h"

void UTDGA_StoneEdge::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UTDGA_StoneEdge::SpawnStone(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag,
	bool bOverridePitch, float PitchOverride)
{
	// Projectile Actor를 서버에만 스폰시킨다.
	// 서버가 스폰시킨 Projectile Actor는 replicated actor가 되어 클라이언트는 replicated된 버젼을 보게 된다.
	if (false == GetAvatarActorFromActorInfo()->HasAuthority()) return; // 서버가 아닌 경우 예외처리

	FActorSpawnParameters params;
	params.Owner = GetAvatarActorFromActorInfo();
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;//AlwaysSpawn는 있는없든 무조건 Spawn시키라는 의미.

	FTransform transform;
	transform.SetLocation(GetAvatarActorFromActorInfo()->GetActorLocation());
	//transform.AddToTranslation(AuraLoction);//보정 위치
	transform.SetRotation(FQuat(GetAvatarActorFromActorInfo()->GetActorRotation()));

	//GetAvatarActorFromActorInfo()->GetWorld()->SpawnActor<ACAura>(StoneActorClass, transform, params);
}
