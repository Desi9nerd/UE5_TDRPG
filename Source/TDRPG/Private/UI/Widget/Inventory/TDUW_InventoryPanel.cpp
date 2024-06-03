#include "UI/Widget/Inventory/TDUW_InventoryPanel.h"
#include "Character/TDCharacter.h"
#include "Component/TDInventoryComponent.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/ListView.h"
#include "Components/TileView.h"
#include "Kismet/GameplayStatics.h"
#include "UI/Widget/Inventory/TDUW_InventoryEntry.h"
#include "UI/Widget/Inventory/TDUW_InventorySlot.h"

void UTDUW_InventoryPanel::NativeConstruct()
{
	Super::NativeConstruct();

	TDCharacter = Cast<ATDCharacter>(GetOwningPlayer()->GetPawn());

	CreateInventorySlotWidgets();

	Button_Category_Weapon->OnClicked.AddDynamic(this, &ThisClass::OnWeaponButtonClicked);
	Button_Category_Armor->OnClicked.AddDynamic(this, &ThisClass::OnArmorButtonClicked);
	Button_Category_Potion->OnClicked.AddDynamic(this, &ThisClass::OnPotionButtonClicked);
	Button_Category_Food->OnClicked.AddDynamic(this, &ThisClass::OnFoodButtonClicked);
	Button_Category_All->OnClicked.AddDynamic(this, &ThisClass::OnAllButtonClicked);
}

void UTDUW_InventoryPanel::NativeDestruct()
{
	Super::NativeDestruct();
}

void UTDUW_InventoryPanel::CreateInventorySlotWidgets()
{
	checkf(TDCharacter, TEXT("No TDCharacter. Check  UTDUW_Inventory::CreateInventorySlotWidgets()"));

	// SelectedInventoryCategory의 기준으로 Inventory 보이게 하기
	DisplayInventorySlotWidgets();

	TileView_Inventory->SetVisibility(ESlateVisibility::Visible);
}


void UTDUW_InventoryPanel::DisplayInventorySlotWidgets()
{
	TileView_Inventory->ClearListItems();

	int32 AmountOfSlots = TDCharacter->GetInventoryComponent()->GetAmountOfSlots(); // Slot 개수
	EItemCategory SelectedCategory = TDCharacter->GetInventoryComponent()->GetSelectedInventoryCategory();

	TArray<UTDInventorySlot*> SelectedCategoryItems;

	switch (SelectedCategory)
	{
	case EItemCategory::Weapon:
		SelectedCategoryItems = TDCharacter->GetInventoryComponent()->WeaponInventoryCategory;
		break;
	case EItemCategory::Armor:
		SelectedCategoryItems = TDCharacter->GetInventoryComponent()->ArmorInventoryCategory;
		break;
	case EItemCategory::Potion:
		SelectedCategoryItems = TDCharacter->GetInventoryComponent()->PotionInventoryCategory;
		break;
	case EItemCategory::Food:
		SelectedCategoryItems = TDCharacter->GetInventoryComponent()->FoodInventoryCategory;
		break;
	default:
		checkNoEntry(); // 선택된 카테고리가 유효하지 않은 경우 체크
		break;
	}

	/*for (int32 i = 0; i < AmountOfSlots; i++)
	{
		UTDItemLibrary* EntryObject = NewObject<UTDItemLibrary>();
		EntryObject->InventorySlot = (*SelectedCategoryItems)[i];

		TileView_Inventory->AddItem(EntryObject);
	}*/

	TileView_Inventory->SetListItems(SelectedCategoryItems);
}


void UTDUW_InventoryPanel::OnWeaponButtonClicked()
{
	if (TDCharacter->GetInventoryComponent()->GetSelectedInventoryCategory() == EItemCategory::Weapon) return;
	UE_LOG(LogTemp, Warning, TEXT("OnWeaponButtonClicked!!"));

	TDCharacter->GetInventoryComponent()->SetSelectedInventoryCategory(EItemCategory::Weapon);

	DisplayInventorySlotWidgets();
}

void UTDUW_InventoryPanel::OnArmorButtonClicked()
{
	if (TDCharacter->GetInventoryComponent()->GetSelectedInventoryCategory() == EItemCategory::Armor) return;
	UE_LOG(LogTemp, Warning, TEXT("OnArmorButtonClicked!!"));

	TDCharacter->GetInventoryComponent()->SetSelectedInventoryCategory(EItemCategory::Armor);

	DisplayInventorySlotWidgets();
}

void UTDUW_InventoryPanel::OnPotionButtonClicked()
{
	if (TDCharacter->GetInventoryComponent()->GetSelectedInventoryCategory() == EItemCategory::Potion) return;
	UE_LOG(LogTemp, Warning, TEXT("OnPotionButtonClicked!!"));

	TDCharacter->GetInventoryComponent()->SetSelectedInventoryCategory(EItemCategory::Potion);

	DisplayInventorySlotWidgets();
}

void UTDUW_InventoryPanel::OnFoodButtonClicked()
{
	if (TDCharacter->GetInventoryComponent()->GetSelectedInventoryCategory() == EItemCategory::Food) return;
	UE_LOG(LogTemp, Warning, TEXT("OnFoodButtonClicked!!"));

	TDCharacter->GetInventoryComponent()->SetSelectedInventoryCategory(EItemCategory::Food);

	DisplayInventorySlotWidgets();
}

void UTDUW_InventoryPanel::OnAllButtonClicked()
{
	
}
