#include "Component/TDInventoryComponent.h"

#include "Actor/TDItemActor.h"
#include "Character/TDCharacter.h"
#include "Components/GridPanel.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Player/TDPlayerController.h"
#include "UI/Widget/Inventory/TDUW_Inventory.h"
#include "UI/Widget/Inventory/TDUW_InventorySlot.h"

UTDInventoryComponent::UTDInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UTDInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	TDPlayerController = Cast<ATDPlayerController>(GetWorld()->GetFirstPlayerController());
	check(TDPlayerController);

	InitializeInventory();
	//TDPlayerController->CreateInventoryCategoryWidgets();
}

void UTDInventoryComponent::InitializeInventory()
{
	if (TDPlayerController->IsLocalController())
	{
		Client_InitializeInventory();
	}
}

void UTDInventoryComponent::SetSelectedInventoryCategory(const EItemCategory& InSelectedInventoryCategory)
{
	if (TDPlayerController->IsLocalController())
	{
		Client_SetSelectedInventoryCategory(InSelectedInventoryCategory);
	}
}

void UTDInventoryComponent::PickupItem()
{
	Server_PickupItem();
}

void UTDInventoryComponent::Server_PickupItem_Implementation()
{
	ATDCharacter* TDCharacter = Cast<ATDCharacter>(TDPlayerController->GetCharacter());
	TArray<AActor*> OverlappingActors;
	TDCharacter->SphereCollision->GetOverlappingActors(OverlappingActors, ATDItemActor::StaticClass());

	if (OverlappingActors.Num() > 0)
	{
		UE_LOG(LogTemp, Log, TEXT("Overlapping Actor!"));
	}
}

void UTDInventoryComponent::Client_SetSelectedInventoryCategory_Implementation(const EItemCategory& InSelectedInventoryCategory)
{
	SelectedInventoryCategory = InSelectedInventoryCategory;
}

void UTDInventoryComponent::Client_InitializeInventory_Implementation()
{
	// AmountOfSlots 만큼 슬롯을 만들기위해 TArray 사이즈를 맞춰줌. 
	WeaponCategory.SetNum(AmountOfSlots);
	ArmorCategory.SetNum(AmountOfSlots);
	PotionCategory.SetNum(AmountOfSlots);
	FoodCategory.SetNum(AmountOfSlots);

	//FString Temp = FString::FromInt(WeaponCategory.Num());
	//UE_LOG(LogTemp, Warning, TEXT("WeaponCategory Size = %s"), *Temp);
}

void UTDInventoryComponent::AddtoInventory(ATDItemActor* InItem)
{
	Client_AddtoInventory(InItem);
}

void UTDInventoryComponent::Client_AddtoInventory_Implementation(ATDItemActor* InItem)
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableRef(TEXT("/Game/BP/Inventory/Data/DT_ItemInfo.DT_ItemInfo'"));

	if (const UDataTable* ItemDataTable = DataTableRef.Object)
	{
		FItem* ItemToAddInfo = ItemDataTable->FindRow<FItem>(FName(*(InItem->GetItemName())), FString(""));

		if (ItemToAddInfo->bStackable)
		{
			
		}
		else
		{
			
		}
	}
}

void UTDInventoryComponent::AddItemToInventory(FItem Item, int32 Quantity, UTDUW_InventorySlot* InventorySlot, int32 SlotIdx, TArray<FInventorySlot>& OutInventory)
{
	for (int i = 0; i < OutInventory.Num(); i++)
	{
		OutInventory[i].ItemQuantity = Quantity;
		OutInventory[i].SlotIndex = SlotIdx;
		OutInventory[i].Item = Item;

		if (Item.ItemCategory == SelectedInventoryCategory)
		{
			InventorySlot->UpdateInventorySlotUI(Item, Quantity);
		}
	}
}