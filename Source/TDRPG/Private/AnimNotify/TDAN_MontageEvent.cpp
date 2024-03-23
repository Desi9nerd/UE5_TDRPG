#include "AnimNotify/TDAN_MontageEvent.h"
#include "AbilitySystemBlueprintLibrary.h"

FString UTDAN_MontageEvent::GetNotifyName_Implementation() const
{
	return "Event";
}

void UTDAN_MontageEvent::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	FGameplayEventData EventData;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(MeshComp->GetOwner(), EventTag, EventData);

}