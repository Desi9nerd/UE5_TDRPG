#include "Component/TDInventoryComponent.h"
#include "Actor/TDItemActor.h"
#include "Character/TDCharacter.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetTextLibrary.h"
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

void UTDInventoryComponent::Client_InitializeInventory_Implementation()
{
	// AmountOfSlots 만큼 슬롯을 만들기위해 TArray 사이즈를 맞춰줌. 
	WeaponCategory.SetNum(AmountOfSlots);
	ArmorCategory.SetNum(AmountOfSlots);
	PotionCategory.SetNum(AmountOfSlots);
	FoodCategory.SetNum(AmountOfSlots);
	
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
			//FindPartialStack(InItem, ItemToAddInfo);
			CreateNewStack(InItem, ItemToAddInfo);
		}
		else
		{
			CreateNewStack(InItem, ItemToAddInfo);
		}

		DestroyPickupItem(InItem, true);
	}
}

void UTDInventoryComponent::AddItemToInventory(const FItem& Item, int32 Quantity, UTDUW_InventorySlot* InventorySlot, int32 SlotIdx, TArray<FInventorySlot>* OutInventory)
{
	//if (Item.ItemCategory == SelectedInventoryCategory)
	{
		(*OutInventory)[SlotIdx].InventorySlot->UpdateInventorySlotUI(Item, Quantity); // UI 갱신.
		UE_LOG(LogTemp, Warning, TEXT("Item = %d"), SlotIdx);

		for (int8 i = 0; i < (*OutInventory).Num(); i++)
		{
			UE_LOG(LogTemp, Warning, TEXT("Item = %s"), *(*OutInventory)[i].InventorySlot->GetName());
			//UE_LOG(LogTemp, Warning, TEXT("Item = %d"), *(*OutInventory)[i].ItemQuantity);
			//UE_LOG(LogTemp, Warning, TEXT("Item = %s"), *(*OutInventory)[i].Item.Thumbnail->GetName());
		}
	}
}

// 인벤토리 내 해당 아이템이 있는지 확인. Stack 여부 검사 후 쌓기.
void UTDInventoryComponent::FindPartialStack(ATDItemActor* ItemToAdd, FItem& ItemToAddInfo)
{
	TArray<FInventorySlot>* CategoryArray = nullptr;

	switch (ItemToAddInfo.ItemCategory)
	{
	case EItemCategory::Weapon:
		CategoryArray = &WeaponCategory;
		break;
	case EItemCategory::Armor:
		CategoryArray = &ArmorCategory;
		break;
	case EItemCategory::Potion:
		CategoryArray = &PotionCategory;
		break;
	case EItemCategory::Food:
		CategoryArray = &FoodCategory;
		break;
	default:
		break;
	}

	if (CategoryArray)
	{
		for (int i = 0; i < CategoryArray->Num() - 1; i++)
		{
			if (UKismetTextLibrary::EqualEqual_TextText((*CategoryArray)[i].Item.Name, ItemToAddInfo.Name) &&
				(*CategoryArray)[i].ItemQuantity < ItemToAddInfo.MaxStackSize)
			{
				// 아이템 수량 갱신
				int32 RemainingQuantity = FMath::Max(0, ItemToAdd->GetItemQuantity() - (ItemToAddInfo.MaxStackSize - (*CategoryArray)[i].ItemQuantity));
				SetNewItemQuantity(ItemToAdd, RemainingQuantity);

				// 인벤토리에 아이템 추가
				AddItemToInventory(ItemToAddInfo, (*CategoryArray)[i].ItemQuantity + (ItemToAdd->GetItemQuantity() - RemainingQuantity), (*CategoryArray)[i].InventorySlot, (*CategoryArray)[i].SlotIndex, CategoryArray);

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
void UTDInventoryComponent::CreateNewStack(ATDItemActor* ItemToAdd, FItem& ItemToAddInfo)
{
	TArray<FInventorySlot>* CategoryArray = nullptr;

	switch (ItemToAddInfo.ItemCategory)
	{
	case EItemCategory::Weapon:
		CategoryArray = &WeaponCategory;
		break;
	case EItemCategory::Armor:
		CategoryArray = &ArmorCategory;
		break;
	case EItemCategory::Potion:
		CategoryArray = &PotionCategory;
		break;
	case EItemCategory::Food:
		CategoryArray = &FoodCategory;
		break;
	default:
		break;
	}
	
	if (CategoryArray)
	{
		bool bRelootItem = false;
		int32 i = 0; // 루프 외부에서 i를 선언하여 사용합니다.
		for (; i < CategoryArray->Num() - 1; i++)
		{
			if ((*CategoryArray)[i].ItemQuantity == 0)
			{
				bInventoryIsFull = false;

				if (ItemToAdd->GetItemQuantity() > ItemToAddInfo.MaxStackSize)
				{
					// 아이템 수량 갱신
					int32 RemainingQuantity = FMath::Max(0, ItemToAdd->GetItemQuantity() - (ItemToAddInfo.MaxStackSize - (*CategoryArray)[i].ItemQuantity));
					SetNewItemQuantity(ItemToAdd, RemainingQuantity);

					// 인벤토리에 아이템 추가
					AddItemToInventory(ItemToAddInfo, ItemToAddInfo.MaxStackSize, (*CategoryArray)[i].InventorySlot, (*CategoryArray)[i].SlotIndex, CategoryArray);
					bRelootItem = true;

					if (RemainingQuantity == 0)
						break;
				}
				else
				{
					// 인벤토리에 아이템 추가
					AddItemToInventory(ItemToAddInfo, ItemToAdd->GetItemQuantity(), (*CategoryArray)[i].InventorySlot, (*CategoryArray)[i].SlotIndex, CategoryArray);
				}
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
				AddItemToInventory(ItemToAddInfo, ItemToAdd->GetItemQuantity(), (*CategoryArray)[i].InventorySlot, (*CategoryArray)[i].SlotIndex, CategoryArray);
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
void UTDInventoryComponent::DropItem(TSubclassOf<ATDItemActor> DropItemClass, int32 ItemQuantity, FTransform SpawnTransform)
{
	if (false == IsValid(DropItemClass)) return;

	Server_DropItem(DropItemClass, ItemQuantity, SpawnTransform);
}

void UTDInventoryComponent::Server_DropItem_Implementation(TSubclassOf<ATDItemActor> DropItemClass, int32 ItemQuantity, FTransform SpawnTransform)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	ATDItemActor* SpawnedItem = GetWorld()->SpawnActor<ATDItemActor>(DropItemClass, SpawnTransform, SpawnParams);

	SpawnedItem->SetItemQuantity(ItemQuantity);
}
//******************************************************************************
