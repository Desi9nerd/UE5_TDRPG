#include "GAS/GameplayAbility/TDGA_DamageCombo.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Character/TDBaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

UTDGA_DamageCombo::UTDGA_DamageCombo()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UTDGA_DamageCombo::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ATDBaseCharacter* TDBaseCharacter = CastChecked<ATDBaseCharacter>(ActorInfo->AvatarActor.Get());
	TDBaseCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	UAbilityTask_PlayMontageAndWait* PlayAttackTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayAttack"), ComboActionMontage, 1.0f, GetNextSection());
	PlayAttackTask->OnCompleted.AddDynamic(this, &ThisClass::OnCompleteCallback);
	PlayAttackTask->OnInterrupted.AddDynamic(this, &ThisClass::OnInterruptedCallback);
	PlayAttackTask->ReadyForActivation();

	StartComboTimer();
}

void UTDGA_DamageCombo::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (false == ComboTimerHandle.IsValid()) // 마지막 공격인 경우
	{
		HasNextComboInput = false; // 콤보 공격이 이어지지 않도록 false 설정
	}
	else // 마지막 공격이 아닌 경우
	{
		HasNextComboInput = true; // 다음 몽타주 색션이 재생되도록 true 설정
	}
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
	CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, MaxComboCount);
	FName NextSection = *FString::Printf(TEXT("%s%d"), *MontageSectionNamePrefix, CurrentCombo);

	return NextSection;
}

void UTDGA_DamageCombo::StartComboTimer() // 콤보 타이머
{
	int32 ComboIndex = CurrentCombo - 1;
	ensure(EffectiveFrameCount.IsValidIndex(ComboIndex));

	const float ComboEffectiveTime = EffectiveFrameCount[ComboIndex] / FrameRate; // 몇 초 이후에 점검할지 변수
	if (ComboEffectiveTime > 0.f)
	{
		GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &ThisClass::CheckComboInput, ComboEffectiveTime, false);
	}
}

void UTDGA_DamageCombo::CheckComboInput()
{
	ComboTimerHandle.Invalidate(); // 타이머 핸들 무력화

	if (HasNextComboInput) // 다음 공격 입력이 있다면
	{
		MontageJumpToSection(GetNextSection()); // 다음 몽타주 색션으로 넘어감
		StartComboTimer(); // 콤보 타이머 새로 시작
		HasNextComboInput = false;
	}
}
