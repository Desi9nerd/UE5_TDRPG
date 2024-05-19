#include "UI/Widget/Inventory/TDUW_InventoryMenu.h"
#include "Character/TDCharacter.h"
#include "Component/TDInventoryComponent.h"
#include "Kismet/GameplayStatics.h"

bool UTDUW_InventoryMenu::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	ATDCharacter* TDCharacter = Cast<ATDCharacter>(GetOwningPlayer()->GetPawn());
	checkf(TDCharacter, TEXT("No TDCharacter. Check  UTDUW_Inventory::CreateInventorySlotWidgets()"));

	//TDCharacter->GetInventoryComponent()->DropItem();

	UGameplayStatics::PlaySoundAtLocation(this, Sound_DropItem, GetOwningPlayer()->K2_GetActorLocation());
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	return true;
}
