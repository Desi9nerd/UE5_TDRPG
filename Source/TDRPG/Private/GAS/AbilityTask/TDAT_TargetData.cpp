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
		AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(SpecHandle, ActivationPredictionKey).AddUObject(this, &UTDAT_TargetData::OnTargetDataReplicatedCallback); // 함수 바인딩

		// TargetData가 이미 보내져 Delegate가 이미 Broadcast되었어도 콜백함수가 호출되도록 조치
		const bool bCalledDelegate = AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(SpecHandle, ActivationPredictionKey);
		if (false == bCalledDelegate) 
		{
			SetWaitingOnRemotePlayerData();
		}
	}

}

void UTDAT_TargetData::SendMouseCursorData() // 서버에 마우스 커서 데이터 보내기 && Locally Broadcast
{
	FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent.Get());

	APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get();

	FHitResult CursorHit; // 마우스 커서 충돌
	PC->GetHitResultUnderCursor(ECC_Target, false, CursorHit); // 마우스 커서 위치

	FGameplayAbilityTargetDataHandle DataHandle;
	FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit(); // Target data with a single hit result, data is packed into the hit result
	Data->HitResult = CursorHit;
	DataHandle.Add(Data);

	MouseCursorLocation = CursorHit.ImpactPoint;

	// 서버에 TargetData 보내기
	AbilitySystemComponent->ServerSetReplicatedTargetData(
		GetAbilitySpecHandle(),
		GetActivationPredictionKey(),
		DataHandle,
		FGameplayTag(),
		AbilitySystemComponent->ScopedPredictionKey);

	// ValidData를 (Locally) Broadcast 하기 (=로컬 클라이언트에서 마우스 커서 데이터 볼 수 있도록 Broadcast)
	if (ShouldBroadcastAbilityTaskDelegates()) // ability is still active 하다면
	{
		ValidData.Broadcast(DataHandle);
	}
}

void UTDAT_TargetData::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag)
{
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey()); // TargetData를 받았다면 저장하고 있지 말아라는 의미

	// ValidData를 (Server) Broadcast 하기
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
}
