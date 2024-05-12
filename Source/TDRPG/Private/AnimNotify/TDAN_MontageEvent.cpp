#include "AnimNotify/TDAN_MontageEvent.h"
#include "AbilitySystemBlueprintLibrary.h"

UTDAN_MontageEvent::UTDAN_MontageEvent()
{
	ComboAttackLevel = 1.0f;
}

FString UTDAN_MontageEvent::GetNotifyName_Implementation() const
{
	return "Event";
}

void UTDAN_MontageEvent::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		AActor* OwnerActor = MeshComp->GetOwner();
		if (IsValid(OwnerActor))
		{
			FGameplayEventData EventData;
			EventData.EventMagnitude = ComboAttackLevel;
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(MeshComp->GetOwner(), EventTag, EventData);
		}
	}
}