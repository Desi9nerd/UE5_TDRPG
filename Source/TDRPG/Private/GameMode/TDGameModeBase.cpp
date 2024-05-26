#include "GameMode/TDGameModeBase.h"
#include "Character/TDCharacter.h"
#include "Component/TDInventoryComponent.h"

// �ʿ� ���ٸ� ���߿� �����!
void ATDGameModeBase::RequestDropItem(APlayerController* InPlayerController, TSubclassOf<ATDItemActor> ItemActor, int32 ItemQuantity, FVector SpawnLocation)
{
	ATDCharacter* TDCharacter = Cast<ATDCharacter>(InPlayerController->GetCharacter());
	checkf(TDCharacter, TEXT("No TDCharacter, Check ATDGameModeBase::RequestDropItem"));

	TDCharacter->GetInventoryComponent()->DropItem(ItemActor, ItemQuantity, SpawnLocation);
}
