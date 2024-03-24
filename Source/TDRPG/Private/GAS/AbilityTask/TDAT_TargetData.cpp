#include "GAS/AbilityTask/TDAT_TargetData.h"

UTDAT_TargetData* UTDAT_TargetData::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UTDAT_TargetData* MyTDAbilityTask = NewAbilityTask<UTDAT_TargetData>(OwningAbility);

	return MyTDAbilityTask;
}

void UTDAT_TargetData::Activate()
{
	Super::Activate();

	APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get();

	FHitResult CursorHit; // 마우스 커서 충돌
	PC->GetHitResultUnderCursor(ECC_Visibility, false, CursorHit); // 마우스커서 위치에 PC가 있으면 CursorHit 정보 업데이트

	ValidData.Broadcast(CursorHit.Location);
}
