#include "UI/Widget/Inventory/TDUW_InventoryPanel.h"
#include "Character/TDCharacter.h"
#include "Component/TDInventoryComponent.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/ListView.h"
#include "Components/TileView.h"
#include "Kismet/GameplayStatics.h"
#include "UI/Widget/Inventory/TDUW_InventoryEntry.h"

void UTDUW_InventoryPanel::NativeConstruct()
{
	Super::NativeConstruct();

	FTimerHandle Timer;
	GetWorld()->GetTimerManager().SetTimer(Timer, this, &ThisClass::DeferredBindingDelegates, 1.f, false);
}

void UTDUW_InventoryPanel::DeferredBindingDelegates()
{
	TDCharacter = Cast<ATDCharacter>(GetOwningPlayer()->GetPawn());
	TDInventoryComponent = TDCharacter->GetInventoryComponent();
	TotalNumOfSlots = TDCharacter->GetInventoryComponent()->GetAmountOfSlots();

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
	if (false == IsValid(TDCharacter))
	{
		UE_LOG(LogTemp, Error, TEXT("No TDCharacter. Cannot create inventory slot widgets."));
		return;
	}

	if (TDInventoryComponent && TDInventoryComponent->InventoryDisplayItems.Num() > 0)
	{
		TDInventoryComponent->ReloadDisplayItems();
	}

	// 슬롯 배열에 빈 슬롯 추가
	while (TDInventoryComponent->InventoryDisplayItems.Num() < TotalNumOfSlots)
	{
		UTDInventoryDisplayItemObject* EmptySlot = NewObject<UTDInventoryDisplayItemObject>();
		TDInventoryComponent->InventoryDisplayItems.Add(EmptySlot);

	}

	TileView_Inventory->SetListItems(TDInventoryComponent->InventoryDisplayItems);

	// Entry 생성 시 SlotIndex 설정
	for (int32 Index = 0; Index < TileView_Inventory->GetNumItems(); ++Index)
	{
		UTDUW_InventoryEntry* InventoryEntry = Cast<UTDUW_InventoryEntry>(TileView_Inventory->GetEntryWidgetFromItem(TDInventoryComponent->InventoryDisplayItems[Index]));
		if (InventoryEntry)
		{
			InventoryEntry->SetSlotIndex(Index);
		}
	}

	TileView_Inventory->SetVisibility(ESlateVisibility::Visible);
}

void UTDUW_InventoryPanel::DisplayInventorySlotWidgets()
{
	UE_LOG(LogTemp, Warning, TEXT("Log  UTDUW_InventoryPanel::DisplayInventorySlotWidgets() "));

	TDInventoryComponent->ReloadDisplayItems();
	TileView_Inventory->RegenerateAllEntries();

	// Entry 생성 시 SlotIndex 설정
	for (int32 Index = 0; Index < TileView_Inventory->GetNumItems(); ++Index)
	{
		UTDUW_InventoryEntry* InventoryEntry = Cast<UTDUW_InventoryEntry>(TileView_Inventory->GetEntryWidgetFromItem(TDInventoryComponent->InventoryDisplayItems[Index]));
		if (InventoryEntry)
		{
			InventoryEntry->SetSlotIndex(Index);
		}
	}
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
	TDCharacter->GetInventoryComponent()->SetSelectedInventoryCategory(EItemCategory::None);

}
