#include "UI/Widget/Inventory/TDUW_InventoryMenuUI.h"
#include "Character/TDCharacter.h"
#include "Component/TDInventoryComponent.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Player/TDPlayerController.h"
#include "UI/HUD/TDHUD.h"
#include "UI/Widget/Inventory/TDUW_Inventory.h"
#include "UI/Widget/Inventory/TDUW_InventoryPanel.h"
#include "UI/Widget/Inventory/TDUW_InventorySlot.h"

void UTDUW_InventoryMenuUI::NativeConstruct()
{
	Super::NativeConstruct();
	
	Button_InventoryMenu->OnClicked.AddDynamic(this, &ThisClass::OnOpenInventory);
}

bool UTDUW_InventoryMenuUI::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	////****************************************************************************
	//ATDPlayerController* TDPlayerController = Cast<ATDPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	//ATDCharacter* TDCharacter = Cast<ATDCharacter>(GetOwningPlayer()->GetPawn());
	//checkf(TDCharacter, TEXT("No TDCharacter. Check  UTDUW_Inventory::CreateInventorySlotWidgets()"));

	//TArray<FInventorySlot>* SelectedCategoryItems = nullptr;

	//switch (TDCharacter->GetInventoryComponent()->GetSelectedInventoryCategory())
	//{
	//case EItemCategory::Weapon:
	//	//SelectedCategoryItems = &TDCharacter->GetInventoryComponent()->GetWeaponCategory();
	//	break;
	//case EItemCategory::Armor:
	//	//SelectedCategoryItems = &TDCharacter->GetInventoryComponent()->GetArmorCategory();
	//	break;
	//case EItemCategory::Potion:
	//	//SelectedCategoryItems = &TDCharacter->GetInventoryComponent()->GetPotionCategory();
	//	break;
	//case EItemCategory::Food:
	//	//SelectedCategoryItems = &TDCharacter->GetInventoryComponent()->GetFoodCategory();
	//	break;
	//default:
	//	break;
	//}

	////****************************************************************************
	//int32 SlotIdx = TDPlayerController->GetTDHUD()->GetInventoryPanelWidget()->DraggedSlotIndex;

	//TDCharacter->GetInventoryComponent()->DropItem((*SelectedCategoryItems)[SlotIdx].Item.ItemClass, (*SelectedCategoryItems)[SlotIdx].ItemQuantity, TDCharacter->GetActorLocation());

	//// ºóÄ­À¸·Î ¾÷µ¥ÀÌÆ®
	////(*SelectedCategoryItems)[SlotIdx].InventorySlot->UpdateInventorySlotUI(FItem(), 0);
	//(*SelectedCategoryItems)[SlotIdx].Item = FItem();
	//(*SelectedCategoryItems)[SlotIdx].ItemQuantity = 0;

	//UGameplayStatics::PlaySoundAtLocation(this, Sound_DropItem, GetOwningPlayer()->K2_GetActorLocation());
	//SetVisibility(ESlateVisibility::HitTestInvisible);

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
