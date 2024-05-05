#include "UI/Widget/Inventory/TDUW_Inventory.h"
#include "Character/TDCharacter.h"
#include "Component/TDInventoryComponent.h"
#include "Components/Button.h"
#include "Components/GridPanel.h"
#include "Components/Image.h"
#include "Player/TDPlayerController.h"
#include "UI/Widget/Inventory/TDUW_InventorySlot.h"

TSharedRef<SWidget> UTDUW_Inventory::RebuildWidget()
{
	return Super::RebuildWidget();
}

void UTDUW_Inventory::ReleaseSlateResources(bool bReleaseChildren)
{
	//if (ChildWidget)
	//{
	//	ChildWidget->ReleaseSlateResource(bReleaseChildren);
	//	ChildWidget = nullptr;
	//}

	// GC에서 파괴될 때 호출되는 함수.
	Super::ReleaseSlateResources(bReleaseChildren);
}

void UTDUW_Inventory::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (false == IsValid(Grid_Inventory)) {
		UE_LOG(LogTemp, Warning, TEXT("No Grid_Inventory. Check UTDUW_Inventory"));}

	TDCharacter = Cast<ATDCharacter>(GetOwningPlayer()->GetPawn());
	checkf(TDCharacter, TEXT("No TDCharacter. Check  UTDUW_Inventory::CreateInventorySlotWidgets()"));

	CreateInventorySlotWidgets();	
	
	Button_Category_Weapon->OnClicked.AddDynamic(this, &UTDUW_Inventory::OnWeaponButtonClicked);
	Button_Category_Armor->OnClicked.AddDynamic(this, &UTDUW_Inventory::OnArmorButtonClicked);
}

void UTDUW_Inventory::NativeDestruct()
{
	Super::NativeDestruct();
}

void UTDUW_Inventory::CreateInventorySlotWidgets()
{
	checkf(InventorySlotWidgetClass, TEXT("No InventorySlotWidgetClass. Check UTDUW_Inventory::CreateInventorySlotWidgets()"));
	checkf(TDCharacter, TEXT("No TDCharacter. Check  UTDUW_Inventory::CreateInventorySlotWidgets()"));

	int32 AmountOfSlots = TDCharacter->GetInventoryComponent()->GetAmountOfSlots(); // Slot 개수
	EItemCategory SelectedCategory = TDCharacter->GetInventoryComponent()->GetSelectedInventoryCategory();

	TArray<FInventorySlot> SelectedCategoryItems;

	switch (SelectedCategory)
	{
	case EItemCategory::Weapon:
		SelectedCategoryItems = TDCharacter->GetInventoryComponent()->GetWeaponCategory();
		break;
	case EItemCategory::Armor:
		SelectedCategoryItems = TDCharacter->GetInventoryComponent()->GetArmorCategory();
		break;
	case EItemCategory::Potion:
		SelectedCategoryItems = TDCharacter->GetInventoryComponent()->GetPotionCategory();
		break;
	case EItemCategory::Food:
		SelectedCategoryItems = TDCharacter->GetInventoryComponent()->GetFoodCategory();
		break;
	default:
		checkNoEntry(); // 선택된 카테고리가 유효하지 않은 경우 체크
		break;
	}

	
	for (int i = 0; i < AmountOfSlots; i++)
	{
		UUserWidget* Widget = CreateWidget(GetWorld(), InventorySlotWidgetClass);
		UTDUW_InventorySlot* InventorySlotWidget = Cast<UTDUW_InventorySlot>(Widget);

		Grid_Inventory->AddChildToGrid(Widget, i / 4, i % 4);

		// 선택된 카테고리에 따라서 'Item'와 'ItemQuantity'를 업데이트.
		InventorySlotWidget->UpdateInventorySlotUI(SelectedCategoryItems[i].Item, SelectedCategoryItems[i].ItemQuantity);
	}
}

void UTDUW_Inventory::DisplayInventorySlotWidgets()
{
	Grid_Inventory->ClearChildren();

	int32 AmountOfSlots = TDCharacter->GetInventoryComponent()->GetAmountOfSlots(); // Slot 개수
	EItemCategory SelectedCategory = TDCharacter->GetInventoryComponent()->GetSelectedInventoryCategory();

	TArray<FInventorySlot> SelectedCategoryItems;

	switch (SelectedCategory)
	{
	case EItemCategory::Weapon:
		SelectedCategoryItems = TDCharacter->GetInventoryComponent()->GetWeaponCategory();
		break;
	case EItemCategory::Armor:
		SelectedCategoryItems = TDCharacter->GetInventoryComponent()->GetArmorCategory();
		break;
	case EItemCategory::Potion:
		SelectedCategoryItems = TDCharacter->GetInventoryComponent()->GetPotionCategory();
		break;
	case EItemCategory::Food:
		SelectedCategoryItems = TDCharacter->GetInventoryComponent()->GetFoodCategory();
		break;
	default:
		checkNoEntry(); // 선택된 카테고리가 유효하지 않은 경우 체크
		break;
	}
	
	for (int i = 0; i < AmountOfSlots; i++)
	{
		Grid_Inventory->AddChildToGrid(SelectedCategoryItems[i].InventorySlot, i / 4, i % 4);

		// 선택된 카테고리 내의 InventorySlot의 'Item'와 'ItemQuantity'를 업데이트.
		SelectedCategoryItems[i].InventorySlot->UpdateInventorySlotUI(SelectedCategoryItems[i].Item, SelectedCategoryItems[i].ItemQuantity);
	}
}

void UTDUW_Inventory::OnWeaponButtonClicked()
{
	TDCharacter->GetInventoryComponent()->SetSelectedInventoryCategory(EItemCategory::Weapon);
}

void UTDUW_Inventory::OnArmorButtonClicked()
{
	TDCharacter->GetInventoryComponent()->SetSelectedInventoryCategory(EItemCategory::Armor);
}
