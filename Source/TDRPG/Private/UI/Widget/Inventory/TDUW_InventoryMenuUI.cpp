#include "UI/Widget/Inventory/TDUW_InventoryMenuUI.h"
#include "Character/TDCharacter.h"
#include "Component/TDInventoryComponent.h"
#include "Components/Button.h"
#include "GameMode/TDGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Player/TDPlayerController.h"
#include "UI/HUD/TDHUD.h"
#include "UI/Widget/Inventory/TDUW_Inventory.h"
#include "UI/Widget/Inventory/TDUW_InventorySlot.h"

void UTDUW_InventoryMenuUI::NativeConstruct()
{
	Super::NativeConstruct();

	//Button_InventoryMenu->OnClicked.AddDynamic(this, &ThisClass::OnOpenInventory);
}

bool UTDUW_InventoryMenuUI::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	//****************************************************************************
	ATDPlayerController* TDPlayerController = Cast<ATDPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	ATDCharacter* TDCharacter = Cast<ATDCharacter>(GetOwningPlayer()->GetPawn());
	checkf(TDCharacter, TEXT("No TDCharacter. Check  UTDUW_Inventory::CreateInventorySlotWidgets()"));

	TArray<FInventorySlot>* SelectedCategoryItems = nullptr;

	switch (TDCharacter->GetInventoryComponent()->GetSelectedInventoryCategory())
	{
	case EItemCategory::Weapon:
		SelectedCategoryItems = &TDCharacter->GetInventoryComponent()->GetWeaponCategory();
		break;
	case EItemCategory::Armor:
		SelectedCategoryItems = &TDCharacter->GetInventoryComponent()->GetArmorCategory();
		break;
	case EItemCategory::Potion:
		SelectedCategoryItems = &TDCharacter->GetInventoryComponent()->GetPotionCategory();
		break;
	case EItemCategory::Food:
		SelectedCategoryItems = &TDCharacter->GetInventoryComponent()->GetFoodCategory();
		break;
	default:
		break;
	}

	//****************************************************************************
	int32 SlotIdx = TDPlayerController->GetTDHUD()->GetInventoryWidget()->DraggedSlotIndex;

	TDCharacter->GetInventoryComponent()->DropItem((*SelectedCategoryItems)[SlotIdx].Item.ItemClass, (*SelectedCategoryItems)[SlotIdx].ItemQuantity, TDCharacter->GetActorLocation());
	//ATDGameModeBase* GameMode = Cast<ATDGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	//GameMode->RequestDropItem(TDPlayerController, (*SelectedCategoryItems)[SlotIdx].Item.ItemClass, (*SelectedCategoryItems)[SlotIdx].ItemQuantity, TDCharacter->GetActorLocation());


	//for (int32 i = 0; i < SelectedCategoryItems->Num(); i++)
	//{
	//	(*SelectedCategoryItems)[i].Item
	//}

	// ºóÄ­À¸·Î ¾÷µ¥ÀÌÆ®
	(*SelectedCategoryItems)[SlotIdx].InventorySlot->UpdateInventorySlotUI(FItem(), 0);
	(*SelectedCategoryItems)[SlotIdx].Item = FItem();
	(*SelectedCategoryItems)[SlotIdx].ItemQuantity = 0;

	UGameplayStatics::PlaySoundAtLocation(this, Sound_DropItem, GetOwningPlayer()->K2_GetActorLocation());
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	return true;
}

void UTDUW_InventoryMenuUI::OnOpenInventory()
{
	ATDPlayerController* TDPlayerController = Cast<ATDPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	ATDCharacter* TDCharacter = Cast<ATDCharacter>(GetOwningPlayer()->GetPawn());

	// TODO: Open Inventory
	if (TDPlayerController->GetTDHUD()->GetInventoryWidget()->IsVisible())
	{
		TDPlayerController->GetTDHUD()->GetInventoryWidget()->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		TDPlayerController->GetTDHUD()->GetInventoryWidget()->SetVisibility(ESlateVisibility::Visible);
	}
}
