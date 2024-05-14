#include "GAS/GameplayCue/TDGC_Notify_Loop.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/AudioComponent.h"
#include "GameplayTags/TDGameplayTags.h"
#include "Interface/ICombat.h"
#include "Kismet/GameplayStatics.h"

ATDGC_Notify_Loop::ATDGC_Notify_Loop()
{
	// GameplayCue가 제거될 때 자동으로 객체를 파괴하도록 설정. 이렇게 함으로써, GameplayCue가 제거되었을 때 이 객체도 함께 정리된다.
	bAutoDestroyOnRemove = true;

	// GameplayCue 태그 설정
	GameplayCueTag = FTDGameplayTags::GetTDGameplayTags().GameplayCue_Loop;
}

bool ATDGC_Notify_Loop::WhileActive_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters)
{
	//**************************************************************************************
	//** 빔
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

	const IICombat* CombatInterface = Cast<IICombat>(Parameters.SourceObject.Get());
	if (CombatInterface) // 적에 맞는 경우
	{
		const FVector EnemyLocation = Cast<AActor>(Parameters.SourceObject.Get())->GetActorLocation(); // 적 위치
		BeamSystem->SetVariableVec3(BeamEnd, EnemyLocation); // BeamEnd, 적 위치
	}
	else // 바닥이나 벽에 맞은 경우
	{
		BeamSystem->SetVariableVec3(BeamEnd, Parameters.Location); // BeamEnd, 마우스 위치
	}
	//**************************************************************************************


	//**************************************************************************************
	//** 사운드
	LoopSound = UGameplayStatics::SpawnSoundAttached(
		LoopingSound,
		Parameters.TargetAttachComponent.Get(),
		NAME_None,
		FVector(0.f, 0.f, 0.f),
		FRotator(0.f, 0.f, 0.f),
		EAttachLocation::KeepRelativeOffset,
		true,
		1.f,
		1.f,
		0.f
	);
	//**************************************************************************************

	return false;
}

bool ATDGC_Notify_Loop::OnRemove_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters)
{
	if (IsValid(BeamSystem))
	{
		BeamSystem->DestroyComponent();
	}
	if (IsValid(LoopSound))
	{
		LoopSound->FadeOut(0.3f, 0.f, EAudioFaderCurve::SCurve);
	}

	return false;
}
