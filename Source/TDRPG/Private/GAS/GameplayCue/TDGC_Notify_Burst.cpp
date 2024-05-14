#include "GAS/GameplayCue/TDGC_Notify_Burst.h"
#include "GameplayTags/TDGameplayTags.h"
#include "Kismet/GameplayStatics.h"

UTDGC_Notify_Burst::UTDGC_Notify_Burst()
{
	GameplayCueTag = FTDGameplayTags::GetTDGameplayTags().GameplayCue_Burst;
}

bool UTDGC_Notify_Burst::OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const
{
	UGameplayStatics::PlaySoundAtLocation(this, SoundClass, MyTarget->GetActorLocation(), FRotator::ZeroRotator);

	return Super::OnExecute_Implementation(MyTarget, Parameters);
}
