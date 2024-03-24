#include "GAS/AbilityTask/TDAT_TargetData.h"
#include "AbilitySystemComponent.h"

UTDAT_TargetData* UTDAT_TargetData::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UTDAT_TargetData* MyTDAbilityTask = NewAbilityTask<UTDAT_TargetData>(OwningAbility);

	return MyTDAbilityTask;
}

void UTDAT_TargetData::Activate()
{
	Super::Activate();

	const bool bIsLocallyControlled = Ability->GetCurrentActorInfo()->IsLocallyControlled();

	if (bIsLocallyControlled) // Ŭ���̾�Ʈ
	{
		SendMouseCursorData();
	}
	else // ����
	{
	}

}

void UTDAT_TargetData::SendMouseCursorData() // ���콺 Ŀ�� ������ ������
{
	FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent.Get());

	APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get();

	FHitResult CursorHit; // ���콺 Ŀ�� �浹
	PC->GetHitResultUnderCursor(ECC_Visibility, false, CursorHit); // ���콺 Ŀ�� ��ġ

	FGameplayAbilityTargetDataHandle DataHandle;
	FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit(); // Target data with a single hit result, data is packed into the hit result
	Data->HitResult = CursorHit;
	DataHandle.Add(Data);

	// ������ TargetData ������
	AbilitySystemComponent->ServerSetReplicatedTargetData(
		GetAbilitySpecHandle(),
		GetActivationPredictionKey(),
		DataHandle,
		FGameplayTag(),
		AbilitySystemComponent->ScopedPredictionKey);

	// ValidData�� (Locally) Broadcast �ϱ� (=���� Ŭ���̾�Ʈ���� ���콺 Ŀ�� ������ �� �� �ֵ��� Broadcast)
	if (ShouldBroadcastAbilityTaskDelegates()) // ability is still active �ϴٸ�
	{
		ValidData.Broadcast(DataHandle);
	}
}
