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
	Button_Category_Potion->OnClicked.AddDynamic(this, &UTDUW_Inventory::OnPotionButtonClicked);
	Button_Category_Food->OnClicked.AddDynamic(this, &UTDUW_Inventory::OnFoodButtonClicked);
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

	// TDInventoryComponent의 아이템 종류들 가져와서 담기
	TArray<FInventorySlot>* WeaponCategoryItems = &TDCharacter->GetInventoryComponent()->GetWeaponCategory();
	TArray<FInventorySlot>* ArmorCategoryItems = &TDCharacter->GetInventoryComponent()->GetArmorCategory();
	TArray<FInventorySlot>* PotionCategoryItems = &TDCharacter->GetInventoryComponent()->GetPotionCategory();
	TArray<FInventorySlot>* FoodCategoryItems = &TDCharacter->GetInventoryComponent()->GetFoodCategory();

	// 무기, 방어구, 포션, 음식 InventorySlot 생성
	UpdateCategoryItems(WeaponCategoryItems, AmountOfSlots);
	UpdateCategoryItems(ArmorCategoryItems, AmountOfSlots);
	UpdateCategoryItems(PotionCategoryItems, AmountOfSlots);
	UpdateCategoryItems(FoodCategoryItems, AmountOfSlots);

	// SelectedInventoryCategory의 기준으로 Inventory 보이게 하기
	DisplayInventorySlotWidgets();
	
}

void UTDUW_Inventory::UpdateCategoryItems(TArray<FInventorySlot>* CategoryItems, int32 AmountOfSlots)
{
	for (int i = 0; i < AmountOfSlots; i++)
	{
		UUserWidget* Widget = CreateWidget(GetWorld(), InventorySlotWidgetClass);
		UTDUW_InventorySlot* InventorySlotWidget = Cast<UTDUW_InventorySlot>(Widget);

		// 선택된 카테고리에 따라서 'Item'와 'ItemQuantity'를 업데이트.
		InventorySlotWidget->UpdateInventorySlotUI((*CategoryItems)[i].Item, (*CategoryItems)[i].ItemQuantity);

		(*CategoryItems)[i].InventorySlot = InventorySlotWidget;
	}
}

void UTDUW_Inventory::DisplayInventorySlotWidgets()
{
	Grid_Inventory->ClearChildren();

	int32 AmountOfSlots = TDCharacter->GetInventoryComponent()->GetAmountOfSlots(); // Slot 개수
	EItemCategory SelectedCategory = TDCharacter->GetInventoryComponent()->GetSelectedInventoryCategory();

	TArray<FInventorySlot>* SelectedCategoryItems = nullptr;

	switch (SelectedCategory)
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
		checkNoEntry(); // 선택된 카테고리가 유효하지 않은 경우 체크
		break;
	}
	
	for (int i = 0; i < AmountOfSlots; i++)
	{
		// Grid_Inventory에 선택된 카테고리 아이템들이 보이게 하기.
		Grid_Inventory->AddChildToGrid((*SelectedCategoryItems)[i].InventorySlot, i / 4, i % 4);
	}
}

void UTDUW_Inventory::OnWeaponButtonClicked()
{
	if (TDCharacter->GetInventoryComponent()->GetSelectedInventoryCategory() == EItemCategory::Weapon) return;

	TDCharacter->GetInventoryComponent()->SetSelectedInventoryCategory(EItemCategory::Weapon);

	DisplayInventorySlotWidgets();
}

void UTDUW_Inventory::OnArmorButtonClicked()
{
	if (TDCharacter->GetInventoryComponent()->GetSelectedInventoryCategory() == EItemCategory::Armor) return;

	TDCharacter->GetInventoryComponent()->SetSelectedInventoryCategory(EItemCategory::Armor);

	DisplayInventorySlotWidgets();
}

void UTDUW_Inventory::OnPotionButtonClicked()
{
	if (TDCharacter->GetInventoryComponent()->GetSelectedInventoryCategory() == EItemCategory::Potion) return;

	TDCharacter->GetInventoryComponent()->SetSelectedInventoryCategory(EItemCategory::Potion);

	DisplayInventorySlotWidgets();
}

void UTDUW_Inventory::OnFoodButtonClicked()
{
	if (TDCharacter->GetInventoryComponent()->GetSelectedInventoryCategory() == EItemCategory::Food) return;

	TDCharacter->GetInventoryComponent()->SetSelectedInventoryCategory(EItemCategory::Food);

	DisplayInventorySlotWidgets();
}
