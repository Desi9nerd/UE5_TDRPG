#include "GAS/GameplayCue/TDGC_Notify_Loop.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "GameplayTags/TDGameplayTags.h"

ATDGC_Notify_Loop::ATDGC_Notify_Loop()
{
	// GameplayCue가 제거될 때 자동으로 객체를 파괴하도록 설정. 이렇게 함으로써, GameplayCue가 제거되었을 때 이 객체도 함께 정리된다.
	bAutoDestroyOnRemove = true;

	// GameplayCue 태그 설정
	GameplayCueTag = FTDGameplayTags::GetTDGameplayTags().GameplayCue_Loop;
}

bool ATDGC_Notify_Loop::WhileActive_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters)
{
	BeamSystem = UNiagaraFunctionLibrary::SpawnSystemAttached(
		SystemTemplate,
		Parameters.TargetAttachComponent.Get(),
		SocketName,	 // 부착 위치 이름
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
