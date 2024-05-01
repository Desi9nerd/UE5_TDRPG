#include "UI/Widget/Inventory/TDUW_Inventory.h"
#include "Character/TDCharacter.h"
#include "Component/TDInventoryComponent.h"
#include "Components/Button.h"
#include "Components/GridPanel.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Player/TDPlayerController.h"
#include "UI/Widget/Inventory/TDUW_InventorySlot.h"

TSharedRef<SWidget> UTDUW_Inventory::RebuildWidget()
{
	return Super::RebuildWidget();
}

void UTDUW_Inventory::Set(int32 ShowCategoryID)
{
	// Data Driven. 좋다. 하지만 성능은..
	// DataTable 사용. 

	//Row = Table->GetRow(ShowCategoryID);
	//if (Row)
	//{
	//	TArray<int32> CategoryIDs = Row->YouhavetoMakeTheseCategories();
	//	TArray<FListViewEntries> Entries;
	//
	//	for (auto CategoryID : CategoryIDs)
	//	{
	//		VerticalListView.
	//		Add(createEntry(CategoryID));
	//	}
	//
	//	VerticalListView.SetEntry(Entries);
	//}
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

	APlayerController* PlayerController = GetOwningPlayer();
	TDCharacter = Cast<ATDCharacter>(PlayerController->GetCharacter());
	if (TDCharacter)
	{
		CreateInventorySlotWidgets();	
	}

	if (Button_Category_Weapon)
	{
		Button_Category_Weapon->OnClicked.AddDynamic(this, &UTDUW_Inventory::OnWeaponButtonClicked);
	}

	if (Button_Category_Armor)
	{
		Button_Category_Armor->OnClicked.AddDynamic(this, &UTDUW_Inventory::OnArmorButtonClicked);
	}
}

void UTDUW_Inventory::NativeDestruct()
{
	Super::NativeDestruct();
}

void UTDUW_Inventory::CreateInventorySlotWidgets()
{
	//checkf(InventorySlotWidgetClass, TEXT("No InventorySlotWidgetClass. Check UTDUW_Inventory::CreateInventorySlotWidgets()"));
	if (false == IsValid(InventorySlotWidgetClass)) return;

	if (IsValid(Grid_Inventory))
	{
		Grid_Inventory->ClearChildren();		
	}

	int32 AmountOfSlots = TDCharacter->GetInventoryComponent()->GetAmountOfSlots(); // Slot 개수
	int32 Cnt = 0;

	for (int i = 0; i < AmountOfSlots; i++)
	{
		UUserWidget* Widget = CreateWidget(GetWorld(), InventorySlotWidgetClass);
		UTDUW_InventorySlot* InventorySlotWidget = Cast<UTDUW_InventorySlot>(Widget);

		Grid_Inventory->AddChildToGrid(Widget, i / 4, i % 4);

		InventorySlotWidget->UpdateInventorySlotUI(TDCharacter->GetInventoryComponent()->GetWeaponCategory()[i].Item, TDCharacter->GetInventoryComponent()->GetWeaponCategory()[i].ItemQuantity);
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
