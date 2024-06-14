﻿#include "Component/TDInventoryComponent.h"
#include "Actor/TDItemActor.h"
#include "Character/TDCharacter.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetTextLibrary.h"
#include "Player/TDPlayerController.h"
#include "UI/Widget/Inventory/TDUW_Inventory.h"
#include "UI/Widget/Inventory/TDUW_InventoryEntry.h"
#include "UI/Widget/Inventory/TDUW_InventorySlot.h"

UTDInventoryComponent::UTDInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);
}

void UTDInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	TDPlayerController = Cast<ATDPlayerController>(GetWorld()->GetFirstPlayerController());
	check(TDPlayerController);

	InitializeInventory();
}

void UTDInventoryComponent::InitializeInventory()
{
	if (TDPlayerController->IsLocalController())
	{
		Client_InitializeInventory();
	}
}

void UTDInventoryComponent::Client_InitializeInventory_Implementation()
{
	// AmountOfSlots 만큼 슬롯을 만들기위해 TArray 사이즈를 맞춰줌.
	// TODO :  생성 이렇게 하는게 맞는지 확인
	WeaponInventoryCategory.Init(NewObject<UTDInventorySlot>(), AmountOfSlots);
	ArmorInventoryCategory.Init(NewObject<UTDInventorySlot>(), AmountOfSlots);
	PotionInventoryCategory.Init(NewObject<UTDInventorySlot>(), AmountOfSlots);
	FoodInventoryCategory.Init(NewObject<UTDInventorySlot>(), AmountOfSlots);

	for (int32 i = 0; i < AmountOfSlots; i++)
	{
		WeaponInventoryCategory[i]->InventorySlot.SlotIndex = i;
		ArmorInventoryCategory[i]->InventorySlot.SlotIndex = i;
		PotionInventoryCategory[i]->InventorySlot.SlotIndex = i;
		FoodInventoryCategory[i]->InventorySlot.SlotIndex = i;
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
	TArray<AActor*> OverlappingActors;
	ATDCharacter* TDCharacter = Cast<ATDCharacter>(GetOwner());
	TDCharacter->GetOverlappingActors(OverlappingActors, ATDItemActor::StaticClass());

	if (OverlappingActors.Num() > 0)
	{
		for (int32 i = 0; i < OverlappingActors.Num(); i++)
		{
			UE_LOG(LogTemp, Log, TEXT("Overlapping Actor!"));

			ATDItemActor* OverlappingItem = Cast<ATDItemActor>(OverlappingActors[i]);
			AddtoInventory(OverlappingItem);			
		}
	}
}

void UTDInventoryComponent::Server_RelootItem_Implementation()
{

}

void UTDInventoryComponent::Client_SetSelectedInventoryCategory_Implementation(const EItemCategory& InSelectedInventoryCategory)
{
	SelectedInventoryCategory = InSelectedInventoryCategory;
}

void UTDInventoryComponent::AddtoInventory(ATDItemActor* InItem)
{
	Client_AddtoInventory(InItem);
}

void UTDInventoryComponent::Client_AddtoInventory_Implementation(ATDItemActor* InItem)
{
	checkf(ItemDataTable, TEXT("No Item DataTable. Check UTDInventoryComponent::Client_AddtoInventory_Implementation"));
	if (ItemDataTable)
	{
		FItem ItemToAddInfo = *(ItemDataTable->FindRow<FItem>(FName(*(InItem->GetItemName())), FString("")));
		
		if (ItemToAddInfo.bStackable) // 인벤토리 내에 해당 아이템이 있는지 찾음.
		{
			if (false == FindPartialStack(InItem, ItemToAddInfo))
			{
				CreateNewStack(InItem, ItemToAddInfo);
			}
		}
		else
		{
			CreateNewStack(InItem, ItemToAddInfo);
		}

		DestroyPickupItem(InItem, true);

		UGameplayStatics::PlaySound2D(this, Sound_DestroyLootedItem);
	}
}

void UTDInventoryComponent::AddItemToInventory(const FItem& Item, int32 Quantity, TArray<UTDInventorySlot*> OutInventorySlots, int32 SlotIdx, TArray<FInventorySlot>* OutInventory)
{
	//if (Item.ItemCategory == SelectedInventoryCategory)
	{
		//(*OutInventory)[SlotIdx].Item = Item;
		//(*OutInventory)[SlotIdx].ItemQuantity = Quantity;
		//(*OutInventory)[SlotIdx].InventorySlot->UpdateInventorySlotUI(Item, Quantity); // UI 갱신.

		OutInventorySlots[SlotIdx]->InventorySlot.Item = Item;
		OutInventorySlots[SlotIdx]->InventorySlot.ItemQuantity = Quantity;
		//OutInventorySlots[SlotIdx]->InventorySlot.InventoryEntry->UpdateInventorySlotUI(Item, Quantity);

		AllItems.Add({SlotIdx, OutInventorySlots[SlotIdx]});


		//UE_LOG(LogTemp, Warning, TEXT("Item = %d"), SlotIdx);

		//AllItems.Add(SlotIdx, (*OutInventory)[SlotIdx]);

		//for (int8 i = 0; i < (*OutInventory).Num(); i++)
		//{
		//	UE_LOG(LogTemp, Warning, TEXT("Item = %s"), *(*OutInventory)[i].InventorySlot->GetName());
		//	//UE_LOG(LogTemp, Warning, TEXT("Item = %d"), *(*OutInventory)[i].ItemQuantity);
		//	//UE_LOG(LogTemp, Warning, TEXT("Item = %s"), *(*OutInventory)[i].Item.Thumbnail->GetName());
		//}
	}
}

// 인벤토리 내 해당 아이템이 있는지 확인. Stack 여부 검사 후 쌓기.
// '해당 아이템의 겹쳐진 개수 < 최대 겹침개수'인 경우 true, 아닌 경우 false 리턴.
bool UTDInventoryComponent::FindPartialStack(ATDItemActor* ItemToAdd, FItem& ItemToAddInfo)
{
	TArray<FInventorySlot>* CategoryArray = nullptr;
	TArray<UTDInventorySlot*> Category;

	switch (ItemToAddInfo.ItemCategory)
	{
	case EItemCategory::Weapon:
		Category = WeaponInventoryCategory;
		break;
	case EItemCategory::Armor:
		Category = ArmorInventoryCategory;
		break;
	case EItemCategory::Potion:
		Category = PotionInventoryCategory;
		break;
	case EItemCategory::Food:
		Category = FoodInventoryCategory;
		break;
	default:
		break;
	}

	//if (Category)
	{
		for (int i = 0; i < Category.Num(); i++)
		{
			if (UKismetTextLibrary::EqualEqual_TextText(Category[i]->InventorySlot.Item.Name, ItemToAddInfo.Name) &&
				Category[i]->InventorySlot.ItemQuantity < ItemToAddInfo.MaxStackSize) // '해당 아이템의 겹쳐진 개수 < 최대 겹침개수'인 경우
			{

				if (Category[i]->InventorySlot.ItemQuantity + ItemToAdd->GetItemQuantity() <= ItemToAddInfo.MaxStackSize)
				{
					// 인벤토리에 아이템 추가
					AddItemToInventory(ItemToAddInfo, Category[i]->InventorySlot.ItemQuantity + ItemToAdd->GetItemQuantity(), Category, i, CategoryArray);
				}
				else
				{
					// 아이템 수량 갱신
					int32 RemainingQuantity = FMath::Max(0, ItemToAdd->GetItemQuantity() - (ItemToAddInfo.MaxStackSize - Category[i]->InventorySlot.ItemQuantity));
					SetNewItemQuantity(ItemToAdd, RemainingQuantity);

					// 인벤토리에 아이템 추가
					AddItemToInventory(ItemToAddInfo, Category[i]->InventorySlot.ItemQuantity + (ItemToAdd->GetItemQuantity() - RemainingQuantity), Category, i, CategoryArray);

					if (i < Category.Num() - 1 && RemainingQuantity > 0)
					{
						CreateNewStack(ItemToAdd, ItemToAddInfo);
					}
				}
				
				return true;
			}
		}
	}

	return false;
}

// Stack 새로 만들기 또는 Stack 불가능한 아이템 채우기.
void UTDInventoryComponent::CreateNewStack(ATDItemActor* ItemToAdd, FItem& ItemToAddInfo)
{
	TArray<FInventorySlot>* CategoryArray = nullptr;
	TArray<UTDInventorySlot*> Category;

	switch (ItemToAddInfo.ItemCategory)
	{
	case EItemCategory::Weapon:
		Category = WeaponInventoryCategory;
		break;
	case EItemCategory::Armor:
		Category = ArmorInventoryCategory;
		break;
	case EItemCategory::Potion:
		Category = PotionInventoryCategory;
		break;
	case EItemCategory::Food:
		Category = FoodInventoryCategory;
		break;
	default:
		break;
	}
	
	//if (CategoryArray)
	{
		bool bRelootItem = false;
		int32 i = 0; // 루프 외부에서 i를 선언.
		for (; i < Category.Num(); i++)
		{
			//UE_LOG(LogTemp, Warning, TEXT("ItemSlot = %s"), *(*CategoryArray)[i].InventorySlot->GetName());
			//UE_LOG(LogTemp, Warning, TEXT("ItemActor = %s"), *(*CategoryArray)[i].Item.ItemClass->GetName());
			//UE_LOG(LogTemp, Warning, TEXT("SlotIndex = %d"), (*CategoryArray)[i].SlotIndex);
			//UE_LOG(LogTemp, Warning, TEXT("Item Quantity = %d"), (*CategoryArray)[i].ItemQuantity);
			UE_LOG(LogTemp, Warning, TEXT("SlotIndex = %d"), Category[i]->InventorySlot.SlotIndex);
			UE_LOG(LogTemp, Warning, TEXT("Item Quantity = %d"), Category[i]->InventorySlot.ItemQuantity);

			if (Category[i]->InventorySlot.ItemQuantity == 0)
			{
				bInventoryIsFull = false;

				if (ItemToAdd->GetItemQuantity() > ItemToAddInfo.MaxStackSize)
				{
					// 아이템 수량 갱신
					int32 RemainingQuantity = FMath::Max(0, ItemToAdd->GetItemQuantity() - (ItemToAddInfo.MaxStackSize - Category[i]->InventorySlot.ItemQuantity));
					SetNewItemQuantity(ItemToAdd, RemainingQuantity);

					// 인벤토리에 아이템 추가
					AddItemToInventory(ItemToAddInfo, ItemToAddInfo.MaxStackSize, Category, Category[i]->InventorySlot.SlotIndex, CategoryArray);
					bRelootItem = true;

					if (RemainingQuantity == 0)
						break;
				}
				else
				{
					// 인벤토리에 아이템 추가
					AddItemToInventory(ItemToAddInfo, ItemToAdd->GetItemQuantity(), Category, i, CategoryArray);
				}

				break;
			}
		}

		if (i >= AmountOfSlots)
		{
			bInventoryIsFull = true;
		}

		if (bRelootItem)
		{
			bRelootItem = false;
			Server_RelootItem();
		}
		else
		{
			if (bInventoryIsFull)
			{
				// 인벤토리에 아이템 추가
				AddItemToInventory(ItemToAddInfo, ItemToAdd->GetItemQuantity(), Category, Category[i]->InventorySlot.SlotIndex, CategoryArray);
			}
		}
	}
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
	//InItem->DestroyPickupItem(InDestroyItem); // 삭제예정: 옵션1
	InItem->DestroyItem();
}
//******************************************************************************

//******************************************************************************
// 아이템 드랍
void UTDInventoryComponent::DropItem(TSubclassOf<ATDItemActor> DropItemClass, int32 ItemQuantity, FVector Location)
{
	if (false == IsValid(DropItemClass)) return;

	Server_DropItem(DropItemClass, ItemQuantity, Location);
}

void UTDInventoryComponent::Server_DropItem_Implementation(TSubclassOf<ATDItemActor> DropItemClass, int32 ItemQuantity, FVector Location)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	ATDItemActor* SpawnedItem = GetWorld()->SpawnActor<ATDItemActor>(DropItemClass, FTransform(Location), SpawnParams);

	SpawnedItem->SetItemQuantity(ItemQuantity);
}
//******************************************************************************

//******************************************************************************
// 아이템 소모 (포션, 음식)
void UTDInventoryComponent::ConsumeItem(const FText& ItemName)
{
	if (ItemDataTable)
	{
		//FItem ItemToAddInfo = *(ItemDataTable->FindRow<FItem>(FName(*(InItem->GetItemName())), FString("")));
	}
}

void UTDInventoryComponent::Server_ConsumeItem_Implementation()
{

}
//******************************************************************************
