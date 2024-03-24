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

	FHitResult CursorHit; // ���콺 Ŀ�� �浹
	PC->GetHitResultUnderCursor(ECC_Visibility, false, CursorHit); // ���콺Ŀ�� ��ġ�� PC�� ������ CursorHit ���� ������Ʈ

	ValidData.Broadcast(CursorHit.Location);
}
