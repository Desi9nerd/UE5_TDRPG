#include "GAS/GameplayAbility/TDGA_Dash.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "GameplayTags/TDGameplayTags.h"
#include "GAS/AbilityTask/TDAT_TargetData.h"
#include "Interface/ICombat.h"

UTDGA_Dash::UTDGA_Dash()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerExecution;
}

void UTDGA_Dash::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	CommitAbility(Handle, ActorInfo, ActivationInfo);

	IICombat* CombatInterface = Cast<IICombat>(GetAvatarActorFromActorInfo());
	if (CombatInterface)
	{
		UTDAT_TargetData* TargetDataTask = UTDAT_TargetData::CreateTargetDataUnderMouse(this);
		TargetDataTask->Activate();
		FVector DashDirection = TargetDataTask->MouseCursorLocation;
		CombatInterface->UpdateFacingTargetCPP(DashDirection);

		checkf(DashMontageToPlay, TEXT("No DashMontageToPlay. Check UTDGA_Dash::ActivateAbility"));
		UAbilityTask_PlayMontageAndWait* PlayDashTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("TDGA_Dash"), DashMontageToPlay, 1.0f);
		PlayDashTask->OnCompleted.AddUniqueDynamic(this, &ThisClass::OnEndAbility);
		PlayDashTask->OnInterrupted.AddUniqueDynamic(this, &ThisClass::OnEndAbility);
		PlayDashTask->OnCancelled.AddUniqueDynamic(this, &ThisClass::OnEndAbility);
		PlayDashTask->ReadyForActivation();

		UAbilityTask_WaitGameplayEvent* WaitTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, FTDGameplayTags::GetTDGameplayTags().Event_Montage_Dash);
	}
}

void UTDGA_Dash::OnEndAbility()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = true;

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}
