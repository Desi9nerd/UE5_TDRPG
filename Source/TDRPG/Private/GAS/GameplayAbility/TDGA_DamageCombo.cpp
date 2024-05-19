#include "GAS/GameplayAbility/TDGA_DamageCombo.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Character/TDBaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/TDPlayerController.h"

UTDGA_DamageCombo::UTDGA_DamageCombo()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UTDGA_DamageCombo::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ATDBaseCharacter* TDBaseCharacter = CastChecked<ATDBaseCharacter>(ActorInfo->AvatarActor.Get());
	TDBaseCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	TDPlayerController = CastChecked<ATDPlayerController>(TDBaseCharacter->GetController());

	checkf(ComboActionMontage, TEXT("No ComboActionMontage. Check UTDGA_DamageCombo::ActivateAbility"));
	if(IsValid(TDBaseCharacter))
	{
		UAbilityTask_PlayMontageAndWait* PlayAttackTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayAttack"), ComboActionMontage, 1.0f, GetNextSection());
		PlayAttackTask->OnCompleted.AddUniqueDynamic(this, &ThisClass::OnCompleteCallback);
		PlayAttackTask->OnInterrupted.AddUniqueDynamic(this, &ThisClass::OnInterruptedCallback);
		PlayAttackTask->ReadyForActivation();
	}

	StartComboTimer();
}

void UTDGA_DamageCombo::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (HasNextComboInput && CurrentCombo < MaxComboCount && PreviousCombo != CurrentCombo)
	{
		MontageJumpToSection(GetNextSection());
		StartComboTimer();
		HasNextComboInput = false;
	}
}

void UTDGA_DamageCombo::OnAction(FGameplayEventData Data)
{
	MontageJumpToSection(GetNextSection());
	StartComboTimer();
	HasNextComboInput = false;
}

void UTDGA_DamageCombo::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UTDGA_DamageCombo::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	ATDBaseCharacter* TDBaseCharacter = CastChecked<ATDBaseCharacter>(ActorInfo->AvatarActor.Get());
	TDBaseCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	//** 콤보 초기화	
	CurrentCombo = 0;
	HasNextComboInput = false;
}

void UTDGA_DamageCombo::OnCompleteCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UTDGA_DamageCombo::OnInterruptedCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

FName UTDGA_DamageCombo::GetNextSection()
{
	PreviousCombo = CurrentCombo;
	CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, MaxComboCount);
	FName NextSection = *FString::Printf(TEXT("%s%d"), *MontageSectionNamePrefix, CurrentCombo);

	UE_LOG(LogTemp, Warning, TEXT("Current Combo = %d"), CurrentCombo);

	return NextSection;
}

void UTDGA_DamageCombo::StartComboTimer() // 콤보 타이머
{
	if (ComboTimerHandle.IsValid())
	{
		ComboTimerHandle.Invalidate();
	}
	GetAvatarActorFromActorInfo()->DisableInput(TDPlayerController);

	GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &ThisClass::CheckComboInput, InputThreashold, false);
}

void UTDGA_DamageCombo::CheckComboInput()
{
	HasNextComboInput = true;

	GetAvatarActorFromActorInfo()->EnableInput(TDPlayerController);
}
