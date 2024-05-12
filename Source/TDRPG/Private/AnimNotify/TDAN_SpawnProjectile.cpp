#include "AnimNotify/TDAN_SpawnProjectile.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Character/TDCharacter.h"

FString UTDAN_SpawnProjectile::GetNotifyName_Implementation() const
{
	return "SpawnProjectile";
}

void UTDAN_SpawnProjectile::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		ATDCharacter* TDCharacter = Cast<ATDCharacter>(MeshComp->GetOwner());
		if (IsValid(TDCharacter))
		{
			//TDCharacter->GetAbilitySystemComponent()->gameplay;
			//
			//FGameplayEventData EventData;
			//UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(MeshComp->GetOwner(), EventTag, EventData);
		}
	}
}
