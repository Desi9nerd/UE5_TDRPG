#include "GAS/GameplayAbility/TDGA_DamageMeleeAttack.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include <Abilities/Tasks/AbilityTask_WaitGameplayEvent.h>
#include "Character/TDBaseCharacter.h"
#include "Kismet/GameplayStatics.h"

//UTDGA_DamageMeleeAttack::UTDGA_DamageMeleeAttack()
//{
//
//}
//
//void UTDGA_DamageMeleeAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,	const FGameplayEventData* TriggerEventData)
//{
//	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
//
//	IICombat* CombatInterface = Cast<IICombat>(GetAvatarActorFromActorInfo());
//	if (CombatInterface == nullptr) return;
//
//	FVector TargetLocation = CombatInterface->GetCombatTargetCPP()->GetActorLocation();
//	CombatInterface->SetFacingTarget(TargetLocation);
//
//	TaggedMontage = GetRandomTaggedMontageFromArray(CombatInterface->GetAttackMontagesCPP());
//
//	UAbilityTask_PlayMontageAndWait* PlayDamageMeleeAttackTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("TDGA_HitReact"), TaggedMontage.Montage, 1.0f);
//
//
//	static UAbilityTask_WaitGameplayEvent* WaitGameplayEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, TaggedMontage.MontageTag);
//	WaitGameplayEvent->EventReceived.AddUniqueDynamic(this, &ThisClass::ApplyDamageCustom); // �̹� ��ϵ� �ݹ��Լ��� ������� �ʴ´�.
//	//WaitGameplayEvent->ReadyForActivation();
//
//	PlayDamageMeleeAttackTask->OnCompleted.AddDynamic(this, &ThisClass::EndGameplayAbility);
//	PlayDamageMeleeAttackTask->OnInterrupted.AddDynamic(this, &ThisClass::EndGameplayAbility);
//	PlayDamageMeleeAttackTask->OnCancelled.AddDynamic(this, &ThisClass::EndGameplayAbility);
//	PlayDamageMeleeAttackTask->ReadyForActivation();
//
//}
//
//void UTDGA_DamageMeleeAttack::EndGameplayAbility()
//{
//	bool bReplicatedEndAbility = true;
//	bool bWasCancelled = true;
//	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
//}
//
//void UTDGA_DamageMeleeAttack::ApplyDamageCustom(FGameplayEventData InPayload)
//{
//	IICombat* CombatInterface = Cast<IICombat>(GetAvatarActorFromActorInfo());
//	if (CombatInterface == nullptr) return;
//
//	TArray<AActor*> OutOverlappingActors; // �浹�� ��� ���͵�
//
//	FVector SphereOriginLocation = CombatInterface->GetCombatSocketLocationCPP(TaggedMontage.MontageTag);
//	TArray<AActor*> Ignores;
//	Ignores.Emplace(GetAvatarActorFromActorInfo());
//
//	FCollisionQueryParams SphereParams;
//	SphereParams.AddIgnoredActors(Ignores);
//
//	const UWorld* World = GEngine->GetWorldFromContextObject(GetAvatarActorFromActorInfo(), EGetWorldErrorMode::LogAndReturnNull);
//	if (IsValid(World))
//	{
//		TArray<FOverlapResult> Overlaps;
//		World->OverlapMultiByObjectType(Overlaps, SphereOriginLocation, FQuat::Identity, FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects), FCollisionShape::MakeSphere(Radius), SphereParams);
//		for (FOverlapResult& Overlap : Overlaps)
//		{
//			const bool ImplementsCombatInterface = Overlap.GetActor()->Implements<UICombat>();
//			if (ImplementsCombatInterface)
//			{
//				const bool IsDead = IICombat::Execute_IsDead(Overlap.GetActor());
//				if (false == IsDead)
//				{
//					OutOverlappingActors.AddUnique(IICombat::Execute_GetAvatar(Overlap.GetActor()));
//				}
//			}
//		}
//	}
//
//	// ���� ������ �˻�
//	for(AActor* HittedActor : OutOverlappingActors)
//	{
//		ATDBaseCharacter* Attacker = Cast<ATDBaseCharacter>(GetAvatarActorFromActorInfo());
//		ATDBaseCharacter* HittedCharacter = Cast<ATDBaseCharacter>(GetAvatarActorFromActorInfo());
//		if (Attacker->ActorHasTag("Player") && HittedCharacter->ActorHasTag("Player") ||
//			Attacker->ActorHasTag("Enemy") && HittedCharacter->ActorHasTag("Enemy"))
//		{
//			continue;
//		}
//
//		CauseDamage(HittedActor); // ������ ����
//	}
//
//	bool bReplicatedEndAbility = true;
//	bool bWasCancelled = true;
//	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
//}
