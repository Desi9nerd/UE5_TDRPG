#include "GAS/GameplayCue/TDGC_Notify_Loop.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "GameplayTags/TDGameplayTags.h"

ATDGC_Notify_Loop::ATDGC_Notify_Loop()
{
	// GameplayCue�� ���ŵ� �� �ڵ����� ��ü�� �ı��ϵ��� ����. �̷��� �����ν�, GameplayCue�� ���ŵǾ��� �� �� ��ü�� �Բ� �����ȴ�.
	bAutoDestroyOnRemove = true;

	// GameplayCue �±� ����
	GameplayCueTag = FTDGameplayTags::GetTDGameplayTags().GameplayCue_Loop;
}

bool ATDGC_Notify_Loop::WhileActive_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters)
{
	BeamSystem = UNiagaraFunctionLibrary::SpawnSystemAttached(
		SystemTemplate,
		Parameters.TargetAttachComponent.Get(),
		SocketName,	 // ���� ��ġ �̸�
		FVector(0.f, 0.f, 0.f),	
		FRotator(0.f, 0.f, 0.f),
		EAttachLocation::KeepRelativeOffset,
		true,
		true,
		ENCPoolMethod::None,
		true
		);

	BeamSystem->SetNiagaraVariableVec3(BeamEnd, Parameters.Location);

	return false;
}

bool ATDGC_Notify_Loop::OnRemove_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters)
{
	if (IsValid(BeamSystem))
	{
		BeamSystem->DestroyComponent();
	}

	return false;
}
