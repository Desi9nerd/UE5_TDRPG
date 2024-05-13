#include "GAS/GameplayCue/TDGC_Notify_Burst.h"
#include "Kismet/GameplayStatics.h"

bool UTDGC_Notify_Burst::OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const
{
	UGameplayStatics::PlaySoundAtLocation(this, SoundClass, MyTarget->GetActorLocation(), FRotator::ZeroRotator);

	return Super::OnExecute_Implementation(MyTarget, Parameters);
}
