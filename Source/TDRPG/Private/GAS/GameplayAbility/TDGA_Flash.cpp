#include "GAS/GameplayAbility/TDGA_Flash.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "GameplayTags/TDGameplayTags.h"
#include "GAS/AbilityTask/TDAT_TargetData.h"
#include "Interface/ICombat.h"

UTDGA_Flash::UTDGA_Flash()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerExecution;
}

void UTDGA_Flash::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	CommitAbility(Handle, ActorInfo, ActivationInfo);
	IICombat* CombatInterface = Cast<IICombat>(GetAvatarActorFromActorInfo());
	if (CombatInterface)
	{
		checkf(FlashMontageToPlay, TEXT("No FlashMontageToPlay. Check UTDGA_Flash::ActivateAbility"));

		static UAbilityTask_WaitGameplayEvent* WaitTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, FTDGameplayTags::GetTDGameplayTags().Event_Montage_Flash);
		WaitTask->EventReceived.AddUniqueDynamic(this, &UTDGA_Flash::OnFlashTeleport);
		WaitTask->ReadyForActivation();

		UAbilityTask_PlayMontageAndWait* PlayDashTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("TDGA_Flash"), FlashMontageToPlay, 1.0f);
		PlayDashTask->OnCompleted.AddUniqueDynamic(this, &ThisClass::OnEndAbility);
		PlayDashTask->OnInterrupted.AddUniqueDynamic(this, &ThisClass::OnEndAbility);
		PlayDashTask->OnCancelled.AddUniqueDynamic(this, &ThisClass::OnEndAbility);
		PlayDashTask->ReadyForActivation();
	}
}

void UTDGA_Flash::OnFlashTeleport(FGameplayEventData Payload)
{
	UTDAT_TargetData* TargetDataTask = UTDAT_TargetData::CreateTargetDataUnderMouse(this);
	TargetDataTask->Activate();
	TargetDataTask->ReadyForActivation();

	FVector_NetQuantize FlashLocation = TargetDataTask->MouseCursorLocation;
	float Zposition = GetAvatarActorFromActorInfo()->GetActorLocation().Z + 10.f;
	
	GetAvatarActorFromActorInfo()->SetActorLocation(FVector(FlashLocation.X, FlashLocation.Y, Zposition));
}

void UTDGA_Flash::OnEndAbility()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = true;

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}
