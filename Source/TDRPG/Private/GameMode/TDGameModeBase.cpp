#include "GameMode/TDGameModeBase.h"
#include "Character/TDCharacter.h"
#include "Component/TDInventoryComponent.h"

// 필요 없다면 나중에 지우기!
void ATDGameModeBase::RequestDropItem(APlayerController* InPlayerController, TSubclassOf<ATDItemActor> ItemActor, int32 ItemQuantity, FVector SpawnLocation)
{
	ATDCharacter* TDCharacter = Cast<ATDCharacter>(InPlayerController->GetCharacter());
	checkf(TDCharacter, TEXT("No TDCharacter, Check ATDGameModeBase::RequestDropItem"));

	TDCharacter->GetInventoryComponent()->DropItem(ItemActor, ItemQuantity, SpawnLocation);
}
