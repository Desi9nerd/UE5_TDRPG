#include "GAS/GameplayAbility/TDGA_DamageMeleeAttack.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include <Abilities/Tasks/AbilityTask_WaitGameplayEvent.h>
#include "Kismet/GameplayStatics.h"
#include "Character/TDBaseCharacter.h"
#include "GAS/TDAbilitySystemBPLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "BlueprintGameplayTagLibrary.h"
#include "GameplayTags/TDGameplayTags.h"

//UTDGA_DamageMeleeAttack::UTDGA_DamageMeleeAttack()
//{
//	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
//}
//
//void UTDGA_DamageMeleeAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
//{
//	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
//
//	bHitTarget = false;
//
//	IICombat* CombatInterface = Cast<IICombat>(GetAvatarActorFromActorInfo());
//	check(CombatInterface);
//	CombatInterface->UpdateFacingTargetCPP(CombatInterface->GetCombatTargetCPP()->GetActorLocation());
//
//	TArray<FTaggedMontage> TaggedMontages = CombatInterface->GetAttackMontagesCPP(); // 공격 몽타주들
//	TaggedMontage = GetRandomTaggedMontageFromArray(TaggedMontages); // 랜덤으로 하나 선택
//
//	UAnimMontage* MontageToPlay = TaggedMontage.Montage;
//	check(MontageToPlay);
//
//	UAbilityTask_PlayMontageAndWait* PlayDamageMeleeAttackTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("TDGA_DamageMeleeAttack"), MontageToPlay, 1.0f);
//
//	static UAbilityTask_WaitGameplayEvent* WaitGameplayEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, TaggedMontage.MontageTag);
//	WaitGameplayEvent->EventReceived.AddUniqueDynamic(this, &ThisClass::ApplyDamageCustom); // 이미 등록된 콜백함수는 등록하지 않는다.
//	WaitGameplayEvent->ReadyForActivation();
//
//	PlayDamageMeleeAttackTask->OnCompleted.AddDynamic(this, &ThisClass::EndGameplayAbility);
//	PlayDamageMeleeAttackTask->OnInterrupted.AddDynamic(this, &ThisClass::EndGameplayAbility);
//	PlayDamageMeleeAttackTask->OnCancelled.AddDynamic(this, &ThisClass::EndGameplayAbility);
//	PlayDamageMeleeAttackTask->ReadyForActivation();
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
//	TArray<AActor*> OutOverlappingActors; // 충돌한 모든 액터들
//
//	FVector SphereOriginLocation = CombatInterface->GetCombatSocketLocationCPP(TaggedMontage.SocketTag);
//	TArray<AActor*> Ignores;
//	Ignores.Emplace(GetAvatarActorFromActorInfo());
//
//	//UTDAbilitySystemBPLibrary::GetLivePlayersWithinRadius(GetAvatarActorFromActorInfo(), OutOverlappingActors, Ignores, 40.f, SphereOriginLocation);
//
//
//	FCollisionQueryParams SphereParams;
//	SphereParams.AddIgnoredActors(Ignores);
//	
//	if (UWorld* World = GEngine->GetWorldFromContextObject(GetAvatarActorFromActorInfo(), EGetWorldErrorMode::LogAndReturnNull))
//	{
//		TArray<FOverlapResult> Overlaps;
//		World->OverlapMultiByObjectType(Overlaps, SphereOriginLocation, FQuat::Identity, FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects), FCollisionShape::MakeSphere(Radius), SphereParams);
//
//		for (FOverlapResult& Overlap : Overlaps)
//		{
//			//const bool ImplementsCombatInterface = Overlap.GetActor()->Implements<UICombat>();
//			//if (ImplementsCombatInterface)
//			//{
//				//const bool IsDead = false; // IICombat::Execute_IsDead(Overlap.GetActor());
//				//if (false == IsDead)
//				//{
//					OutOverlappingActors.AddUnique(Overlap.GetActor());
//					DrawDebugSphere(GetWorld(), Overlap.GetActor()->GetActorLocation(), 40.f, 12, FColor::Orange, false, 2.f, 0.f);
//				//}
//			//}
//		}
//	}
//
//	// 같은 팀인지 검사
//	for (AActor* HittedActor : OutOverlappingActors)
//	{
//		ATDBaseCharacter* Attacker = Cast<ATDBaseCharacter>(GetAvatarActorFromActorInfo());
//		ATDBaseCharacter* HittedCharacter = Cast<ATDBaseCharacter>(HittedActor);
//		if (Attacker->ActorHasTag("Player") && HittedCharacter->ActorHasTag("Player") ||
//			Attacker->ActorHasTag("Enemy") && HittedCharacter->ActorHasTag("Enemy"))
//		{
//			continue;
//		}
//
//		CauseDamage(HittedActor); // 데미지 전달
//		bHitTarget = true;
//		CombatInterface->GetBloodEffectCPP();
//	}
//
//	if(bHitTarget)
//	{
//		FGameplayEffectContextHandle EffectContext;
//		FGameplayTag MatchedTagName;
//		FGameplayTag OriginalTag;
//		AActor* SourceObject = CombatInterface->GetCombatTargetCPP();
//		FGameplayTagContainer AggregatedSourceTags = UBlueprintGameplayTagLibrary::MakeGameplayTagContainerFromTag(TaggedMontage.MontageTag);
//		FGameplayTagContainer AggregatedTargetTags;
//		UPhysicalMaterial* PhysicalMaterial = nullptr;
//		USceneComponent* TargetAttachComponent = nullptr;
//
//		FGameplayCueParameters GameplayCueParameters = UAbilitySystemBlueprintLibrary::MakeGameplayCueParameters(
//			0.f,
//			0.f,
//			EffectContext,
//			MatchedTagName,
//			OriginalTag,
//			AggregatedSourceTags,
//			AggregatedTargetTags,
//			SphereOriginLocation,
//			FVector(0.f, 0.f, 0.f),
//			GetAvatarActorFromActorInfo(),
//			GetAvatarActorFromActorInfo(),
//			SourceObject,
//			PhysicalMaterial,
//			1,
//			1,
//			TargetAttachComponent,
//			true
//			);
//
//
//		FGameplayTag GameplayCueTag = FTDGameplayTags::GetTDGameplayTags().GameplayCue_MeleeImpact;
//		K2_ExecuteGameplayCueWithParams(GameplayCueTag, GameplayCueParameters);
//	}
//
//	EndGameplayAbility();
//}
