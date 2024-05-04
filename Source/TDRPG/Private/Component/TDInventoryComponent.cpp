#include "Component/TDInventoryComponent.h"

#include "Actor/TDItemActor.h"
#include "Character/TDCharacter.h"
#include "Components/GridPanel.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetTextLibrary.h"
#include "Player/TDPlayerController.h"
#include "UI/Widget/Inventory/TDUW_Inventory.h"
#include "UI/Widget/Inventory/TDUW_InventorySlot.h"

UTDInventoryComponent::UTDInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

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

		ATDItemActor* OverlappingItem = Cast<ATDItemActor>(OverlappingActors[0]);
		AddtoInventory(OverlappingItem);
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
	ItemToAdd = InItem;

	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableRef(TEXT("/Game/BP/Inventory/Data/DT_ItemInfo.DT_ItemInfo'"));

	if (const UDataTable* ItemDataTable = DataTableRef.Object)
	{
		FItem ItemToAddInfo = *(ItemDataTable->FindRow<FItem>(FName(*(InItem->GetItemName())), FString("")));
		
		if (ItemToAddInfo.bStackable) // 인벤토리 내에 해당 아이템이 있는지 찾음.
		{
			FindPartialStack(ItemToAddInfo);
		}
		else
		{
			CreateNewStack(ItemToAddInfo);
		}
	}
}

void UTDInventoryComponent::AddItemToInventory(FItem Item, int32 Quantity, UTDUW_InventorySlot* InventorySlot, int32 SlotIdx, TArray<FInventorySlot>& OutInventory)
{
	for (int i = 0; i < OutInventory.Num(); i++)
	{
		if (OutInventory[i].SlotIndex == SlotIdx)
		{
			OutInventory[i].ItemQuantity = Quantity; // 아이템 수량 갱신.
			OutInventory[i].Item = Item; // 아이템 정보 갱신.

			if (Item.ItemCategory == SelectedInventoryCategory)
			{
				InventorySlot->UpdateInventorySlotUI(Item, Quantity); // UI 갱신.
			}
			break;
		}
	}
}

// 인벤토리 내 해당 아이템이 있는지 확인. Stack 여부 검사 후 쌓기.
void UTDInventoryComponent::FindPartialStack(FItem& ItemToAddInfo)
{
	if (ItemToAddInfo.ItemCategory == EItemCategory::Weapon)
	{
		for (int i = 0; i < WeaponCategory.Num(); i++)
		{
			if (UKismetTextLibrary::EqualEqual_TextText(WeaponCategory[i].Item.Name, ItemToAddInfo.Name) &&
				WeaponCategory[i].ItemQuantity < ItemToAddInfo.MaxStackSize)
			{
				// 아이템 수량 갱신
				int32 RemainingQuantity = FMath::Max(0, ItemToAdd->GetItemQuantity() - (ItemToAddInfo.MaxStackSize - WeaponCategory[i].ItemQuantity));
				SetNewItemQuantity(ItemToAdd, RemainingQuantity);

				// 인벤토리에 아이템 추가
				UTDUW_InventorySlot* Temp = Cast<UTDUW_InventorySlot>(WeaponCategory[i].InventorySlot);
				AddItemToInventory(ItemToAddInfo, WeaponCategory[i].ItemQuantity + (ItemToAdd->GetItemQuantity() - RemainingQuantity), Temp, WeaponCategory[i].SlotIndex, WeaponCategory);

				if (RemainingQuantity == 0)
					break;
			}

			if (i >= AmountOfSlots)
			{
				break;
			}
		}
	}
	else if (ItemToAddInfo.ItemCategory == EItemCategory::Armor)
	{
		for (int i = 0; i < ArmorCategory.Num(); i++)
		{
			if (UKismetTextLibrary::EqualEqual_TextText(ArmorCategory[i].Item.Name, ItemToAddInfo.Name) &&
				ArmorCategory[i].ItemQuantity < ItemToAddInfo.MaxStackSize)
			{
				// 아이템 수량 갱신
				int32 RemainingQuantity = FMath::Max(0, ItemToAdd->GetItemQuantity() - (ItemToAddInfo.MaxStackSize - ArmorCategory[i].ItemQuantity));
				SetNewItemQuantity(ItemToAdd, RemainingQuantity);

				// 인벤토리에 아이템 추가
				UTDUW_InventorySlot* Temp = Cast<UTDUW_InventorySlot>(ArmorCategory[i].InventorySlot);
				AddItemToInventory(ItemToAddInfo, ArmorCategory[i].ItemQuantity + (ItemToAdd->GetItemQuantity() - RemainingQuantity), Temp, ArmorCategory[i].SlotIndex, ArmorCategory);

				if (RemainingQuantity == 0)
					break;
			}

			if (i >= AmountOfSlots)
			{
				break;
			}
		}
	}
	else if (ItemToAddInfo.ItemCategory == EItemCategory::Potion)
	{
		for (int i = 0; i < PotionCategory.Num(); i++)
		{
			if (UKismetTextLibrary::EqualEqual_TextText(PotionCategory[i].Item.Name, ItemToAddInfo.Name) &&
				PotionCategory[i].ItemQuantity < ItemToAddInfo.MaxStackSize)
			{
				// 아이템 수량 갱신
				int32 RemainingQuantity = FMath::Max(0, ItemToAdd->GetItemQuantity() - (ItemToAddInfo.MaxStackSize - PotionCategory[i].ItemQuantity));
				SetNewItemQuantity(ItemToAdd, RemainingQuantity);

				// 인벤토리에 아이템 추가
				UTDUW_InventorySlot* Temp = Cast<UTDUW_InventorySlot>(PotionCategory[i].InventorySlot);
				AddItemToInventory(ItemToAddInfo, PotionCategory[i].ItemQuantity + (ItemToAdd->GetItemQuantity() - RemainingQuantity), Temp, PotionCategory[i].SlotIndex, PotionCategory);

				if (RemainingQuantity == 0)
					break;
			}

			if (i >= AmountOfSlots)
			{
				break;
			}
		}
	}
	else if (ItemToAddInfo.ItemCategory == EItemCategory::Food)
	{
		for (int i = 0; i < FoodCategory.Num(); i++)
		{
			if (UKismetTextLibrary::EqualEqual_TextText(FoodCategory[i].Item.Name, ItemToAddInfo.Name) &&
				FoodCategory[i].ItemQuantity < ItemToAddInfo.MaxStackSize)
			{
				// 아이템 수량 갱신
				int32 RemainingQuantity = FMath::Max(0, ItemToAdd->GetItemQuantity() - (ItemToAddInfo.MaxStackSize - FoodCategory[i].ItemQuantity));
				SetNewItemQuantity(ItemToAdd, RemainingQuantity);

				// 인벤토리에 아이템 추가
				UTDUW_InventorySlot* Temp = Cast<UTDUW_InventorySlot>(FoodCategory[i].InventorySlot);
				AddItemToInventory(ItemToAddInfo, FoodCategory[i].ItemQuantity + (ItemToAdd->GetItemQuantity() - RemainingQuantity), Temp, FoodCategory[i].SlotIndex, FoodCategory);

				if (RemainingQuantity == 0)
					break;
			}

			if (i >= AmountOfSlots)
			{
				break;
			}
		}
	}
}

// Stack 새로 만들기 또는 Stack 불가능한 아이템 채우기.
void UTDInventoryComponent::CreateNewStack(FItem& ItemToAddInfo)
{

}

//******************************************************************************
//** 아이템 수량
void UTDInventoryComponent::SetNewItemQuantity(ATDItemActor* InItem, int32 ItemQuantity)
{
	Server_SetNewItemQuantity(InItem, ItemQuantity);
}
void UTDInventoryComponent::Server_SetNewItemQuantity_Implementation(ATDItemActor* InItem, int32 ItemQuantity)
{
	InItem->SetItemQuantity(ItemQuantity);
}
//******************************************************************************

//******************************************************************************
//** 아이템 주운 후 없애기
void UTDInventoryComponent::DestroyPickupItem(ATDItemActor* InItem, bool InDestroyItem)
{
	Server_DestroyPickupItem(InItem, InDestroyItem);
}

void UTDInventoryComponent::Server_DestroyPickupItem_Implementation(ATDItemActor* InItem, bool InDestroyItem)
{
	InItem->DestroyPickupItem(InDestroyItem);
}
//******************************************************************************