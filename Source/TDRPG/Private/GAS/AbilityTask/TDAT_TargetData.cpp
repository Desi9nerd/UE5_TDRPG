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

	if (bIsLocallyControlled) // 클라이언트
	{
		SendMouseCursorData();
	}
	else // 서버
	{
	}

}

void UTDAT_TargetData::SendMouseCursorData() // 마우스 커서 데이터 보내기
{
	FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent.Get());

	APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get();

	FHitResult CursorHit; // 마우스 커서 충돌
	PC->GetHitResultUnderCursor(ECC_Visibility, false, CursorHit); // 마우스 커서 위치

	FGameplayAbilityTargetDataHandle DataHandle;
	FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit(); // Target data with a single hit result, data is packed into the hit result
	Data->HitResult = CursorHit;
	DataHandle.Add(Data);

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
