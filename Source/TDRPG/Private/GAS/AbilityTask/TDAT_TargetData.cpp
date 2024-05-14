#include "GAS/AbilityTask/TDAT_TargetData.h"
#include "AbilitySystemComponent.h"
#include "TDRPG/TDRPG.h"

UTDAT_TargetData* UTDAT_TargetData::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UTDAT_TargetData* MyTDAbilityTask = NewAbilityTask<UTDAT_TargetData>(OwningAbility);

	return MyTDAbilityTask;
}

void UTDAT_TargetData::Activate()
{
	Super::Activate();

	const bool bIsLocallyControlled = Ability->GetCurrentActorInfo()->IsLocallyControlled();

	if (bIsLocallyControlled) // Client.
	{
		SendMouseCursorData();
	}
	else // Server.
	{
		const FGameplayAbilitySpecHandle SpecHandle = GetAbilitySpecHandle();
		const FPredictionKey ActivationPredictionKey = GetActivationPredictionKey();
		AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(SpecHandle, ActivationPredictionKey).AddUObject(this, &UTDAT_TargetData::OnTargetDataReplicatedCallback); // �Լ� ���ε�

		// TargetData�� �̹� ������ Delegate�� �̹� Broadcast�Ǿ�� �ݹ��Լ��� ȣ��ǵ��� ��ġ
		const bool bCalledDelegate = AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(SpecHandle, ActivationPredictionKey);
		if (false == bCalledDelegate) 
		{
			SetWaitingOnRemotePlayerData();
		}
	}

}

void UTDAT_TargetData::SendMouseCursorData() // ������ ���콺 Ŀ�� ������ ������ && Locally Broadcast
{
	FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent.Get());

	APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get();

	FHitResult CursorHit; // ���콺 Ŀ�� �浹
	PC->GetHitResultUnderCursor(ECC_Target, false, CursorHit); // ���콺 Ŀ�� ��ġ

	FGameplayAbilityTargetDataHandle DataHandle;
	FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit(); // Target data with a single hit result, data is packed into the hit result
	Data->HitResult = CursorHit;
	DataHandle.Add(Data);

	MouseCursorLocation = CursorHit.ImpactPoint;

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

void UTDAT_TargetData::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag)
{
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey()); // TargetData�� �޾Ҵٸ� �����ϰ� ���� ���ƶ�� �ǹ�

	// ValidData�� (Server) Broadcast �ϱ�
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
}
