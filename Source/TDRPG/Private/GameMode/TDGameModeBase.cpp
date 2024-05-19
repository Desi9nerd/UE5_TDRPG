#include "GameMode/TDGameModeBase.h"
#include "Character/TDCharacter.h"
#include "Component/TDInventoryComponent.h"

// �ʿ� ���ٸ� ���߿� �����!
void ATDGameModeBase::RequestDropItem(APlayerController* InPlayerController, TSubclassOf<ATDItemActor> ItemActor, int32 ItemQuantity, FTransform SpawnTransform)
{
	ATDCharacter* TDCharacter = Cast<ATDCharacter>(InPlayerController);
	checkf(TDCharacter, TEXT("No TDCharacter, Check ATDGameModeBase::RequestDropItem"));

	TDCharacter->GetInventoryComponent()->DropItem(ItemActor, ItemQuantity, SpawnTransform);
}
