#include "UI/Widget/Inventory/TDUW_Inventory.h"

#include "Character/TDCharacter.h"
#include "Component/TDInventoryComponent.h"
#include "GameFramework/PlayerController.h"
#include "Components/GridPanel.h"
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

	if (false == IsValid(VB_Categories)){
		UE_LOG(LogTemp, Warning, TEXT("No VB_Categories. Check UTDUW_Inventory"));}
	if (false == IsValid(Grid_Inventory)) {
		UE_LOG(LogTemp, Warning, TEXT("No Grid_Inventory. Check UTDUW_Inventory"));}

	CreateInventorySlotWidgets();
}

void UTDUW_Inventory::NativeDestruct()
{
	Super::NativeDestruct();
}

void UTDUW_Inventory::CreateInventorySlotWidgets()
{
	//checkf(InventorySlotWidgetClass, TEXT("No InventorySlotWidgetClass. Check UTDUW_Inventory::CreateInventorySlotWidgets()"));

	if(IsValid(Grid_Inventory))
	{
		Grid_Inventory->ClearChildren();		
	}

	ATDPlayerController* TDPlayerController = Cast<ATDPlayerController>(GetOwningPlayer());
	ATDCharacter* TDCharacter = Cast<ATDCharacter>(GetOwningPlayer());

	int32 AmountOfSlots = 16; // TDCharacter->GetInventoryComponent()->AmountOfSlots;

	for (int i = 0; i < AmountOfSlots; i++)
	{
		UUserWidget* Widget = CreateWidget(GetWorld(), InventorySlotWidgetClass);
		UTDUW_InventorySlot* InventorySlotWidget = Cast<UTDUW_InventorySlot>(Widget);
		InventorySlotWidgets.Add(InventorySlotWidget);

		Grid_Inventory->AddChildToGrid(Widget, i / 4, i % 4);
	}
}
