#include "GAS/GameplayAbility/TDGA_HitReact.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Character/TDBaseCharacter.h"

UTDGA_HitReact::UTDGA_HitReact()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerExecution;
}

void UTDGA_HitReact::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UGameplayEffect* GameplayEffectHitReact = Cast<UGameplayEffect>(GameplayEffectClass);
	ActiveGEHitReact = ApplyGameplayEffectToOwner(Handle, ActorInfo, ActivationInfo, GameplayEffectHitReact, 1, 1);

	ATDBaseCharacter* HittedCharacter = Cast<ATDBaseCharacter>(GetAvatarActorFromActorInfo());
	
	if (HittedCharacter)
	{
		TObjectPtr<UAnimMontage> MontageToPlay = HittedCharacter->GetHitReactMontageFromCharacter();
		check(MontageToPlay);

		UAbilityTask_PlayMontageAndWait* PlayHitReactTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("TDGA_HitReact"), MontageToPlay, 1.0f);
		PlayHitReactTask->OnCompleted.AddDynamic(this, &ThisClass::RemoveGameplayEffectFromOwnerWithHandle);
		PlayHitReactTask->OnInterrupted.AddDynamic(this, &ThisClass::RemoveGameplayEffectFromOwnerWithHandle);
		PlayHitReactTask->OnCancelled.AddDynamic(this, &ThisClass::RemoveGameplayEffectFromOwnerWithHandle);
		PlayHitReactTask->ReadyForActivation();
	}
}

void UTDGA_HitReact::RemoveGameplayEffectFromOwnerWithHandle()
{
	BP_RemoveGameplayEffectFromOwnerWithHandle(ActiveGEHitReact);

	bool bReplicatedEndAbility = true;
	bool bWasCancelled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}
