#include "UI/Widget/Inventory/TDUW_InventoryMenuUI.h"
#include "Character/TDCharacter.h"
#include "Component/TDInventoryComponent.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Player/TDPlayerController.h"
#include "UI/HUD/TDHUD.h"
#include "UI/Widget/Inventory/TDUW_Inventory.h"
#include "UI/Widget/Inventory/TDUW_InventoryPanel.h"

void UTDUW_InventoryMenuUI::NativeConstruct()
{
	Super::NativeConstruct();
	
	Button_InventoryMenu->OnClicked.AddDynamic(this, &ThisClass::OnOpenInventory);
}

bool UTDUW_InventoryMenuUI::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	//****************************************************************************
	ATDPlayerController* TDPlayerController = Cast<ATDPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	ATDCharacter* TDCharacter = Cast<ATDCharacter>(GetOwningPlayer()->GetPawn());
	//****************************************************************************
	int32 SlotIdx = TDPlayerController->GetTDHUD()->GetInventoryPanelWidget()->DraggedSlotIndex;

	TDCharacter->GetInventoryComponent()->DropItem(TDCharacter->GetInventoryComponent()->InventoryDisplayItems[SlotIdx]->Data.Item.ItemClass, TDCharacter->GetInventoryComponent()->InventoryDisplayItems[SlotIdx]->Data.ItemQuantity, TDCharacter->GetActorLocation());

	// ºóÄ­À¸·Î ¾÷µ¥ÀÌÆ®
	TDCharacter->GetInventoryComponent()->InventoryDisplayItems[SlotIdx]->Data.Item = FItem();
	TDCharacter->GetInventoryComponent()->InventoryDisplayItems[SlotIdx]->Data.ItemQuantity = 0;

	TDPlayerController->GetTDHUD()->GetInventoryPanelWidget()->DisplayInventorySlotWidgets();

	UGameplayStatics::PlaySoundAtLocation(this, Sound_DropItem, GetOwningPlayer()->K2_GetActorLocation());
	SetVisibility(ESlateVisibility::HitTestInvisible);

	return true;
}

void UTDUW_InventoryMenuUI::OnOpenInventory()
{
	ATDPlayerController* TDPlayerController = Cast<ATDPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	ATDCharacter* TDCharacter = Cast<ATDCharacter>(GetOwningPlayer()->GetPawn());

	// TODO: Open Inventory
	if (TDPlayerController->GetTDHUD()->GetInventoryPanelWidget()->IsVisible())
	{
		TDPlayerController->GetTDHUD()->GetInventoryPanelWidget()->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		TDPlayerController->GetTDHUD()->GetInventoryPanelWidget()->SetVisibility(ESlateVisibility::Visible);
	}
}
