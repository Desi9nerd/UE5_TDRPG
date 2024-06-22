#include "Component/TDInventoryComponent.h"
#include "Actor/TDItemActor.h"
#include "Character/TDCharacter.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetTextLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Player/TDPlayerController.h"
#include "UI/HUD/TDHUD.h"
#include "UI/Widget/Inventory/TDUW_Inventory.h"
#include "UI/Widget/Inventory/TDUW_InventoryPanel.h"

void UTDInventoryComponent::OnRep_Inventory()
{
	if (false == GetOwner()->HasAuthority()) // 클라이언트인 경우에만 실행
	{
		ReloadDisplayItems();
	}
}

void UTDInventoryComponent::ReloadDisplayItems()
{
	UE_LOG(LogTemp, Warning, TEXT("UTDInventoryComponent::ReloadDisplayItems()"));

	for (auto& InventoryItem : Inventory.Items)
	{
		UTDInventoryDisplayItemObject* ItemToAdd = NewObject<UTDInventoryDisplayItemObject>(this, UTDInventoryDisplayItemObject::StaticClass());
		FItem* FoundItem = ItemDataTable->FindRow<FItem>(FName(*((InventoryItem.Item.Name).ToString())), FString(""));

		if (FoundItem)
		{
			// Add details from Setup inventory data (FoundItem from ItemDataTable)
			ItemToAdd->Data.Item = *FoundItem;
			ItemToAdd->Data.ItemQuantity = InventoryItem.ItemQuantity;
			ItemToAdd->Data.SlotIndex = InventoryItem.SlotIndex;

			// Add the item to the array
			InventoryDisplayItems.Add(ItemToAdd);
		}
	}
}

UTDInventoryComponent::UTDInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);
}

void UTDInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams Params;
	Params.bIsPushBased = true;
	Params.Condition = COND_OwnerOnly;
	
	DOREPLIFETIME_WITH_PARAMS_FAST(UTDInventoryComponent, Inventory, Params);
	//DOREPLIFETIME_CONDITION(UTDInventoryComponent, Inventory, COND_OwnerOnly);
}

TObjectPtr<ATDPlayerController> UTDInventoryComponent::GetTDPlayerController()
{
	if (IsValid(TDPlayerController)) return TDPlayerController;

	TDPlayerController = Cast<ATDPlayerController>(GetWorld()->GetFirstPlayerController());
	return TDPlayerController;
}

void UTDInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	TDPlayerController = Cast<ATDPlayerController>(GetWorld()->GetFirstPlayerController());
	check(TDPlayerController);
}

void UTDInventoryComponent::SetSelectedInventoryCategory(const EItemCategory& InSelectedInventoryCategory)
{
	if (TDPlayerController->IsLocalController())
	{
		Client_SetSelectedInventoryCategory(InSelectedInventoryCategory);
	}
}

void UTDInventoryComponent::Client_SetSelectedInventoryCategory_Implementation(const EItemCategory& InSelectedInventoryCategory)
{
	SelectedInventoryCategory = InSelectedInventoryCategory;
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
			//UE_LOG(LogTemp, Log, TEXT("Overlapping Actor!"));
			ATDItemActor* OverlappingItem = Cast<ATDItemActor>(OverlappingActors[i]);
			AddtoInventory(OverlappingItem);			
		}
	}
}

void UTDInventoryComponent::Server_RelootItem_Implementation()
{
}

void UTDInventoryComponent::AddtoInventory(ATDItemActor* InItem)
{
	Client_AddtoInventory(InItem);
}

void UTDInventoryComponent::Client_AddtoInventory_Implementation(ATDItemActor* InItem)
{
	if (Inventory.Items.Num() >= GetAmountOfSlots()) return;

	checkf(ItemDataTable, TEXT("No Item DataTable. Check UTDInventoryComponent::Client_AddtoInventory_Implementation"));
	if (ItemDataTable)
	{
		FItem ItemToAddInfo = *(ItemDataTable->FindRow<FItem>(FName(*(InItem->GetItemName())), FString("")));
		
		if (ItemToAddInfo.bStackable) // 인벤토리 내에 해당 아이템이 있는지 찾음.
		{
			//if (false == FindPartialStack(InItem, ItemToAddInfo))
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

void UTDInventoryComponent::AddItemToInventory(const FItem& Item, int32 Quantity)
{
	// 빈 슬롯을 찾아서 사용
	for (int32 i = 0; i < InventoryDisplayItems.Num(); i++)
	{
		if (InventoryDisplayItems[i]->Data.Item.Name.IsEmpty())
		{
			InventoryDisplayItems[i]->Data.Item = Item;
			InventoryDisplayItems[i]->Data.ItemQuantity = Quantity;
			InventoryDisplayItems[i]->Data.SlotIndex = i;

			if (Inventory.Items.Num() <= i)
			{
				FInventoryItem ItemToAdd;
				ItemToAdd.Item = Item;
				ItemToAdd.ItemQuantity = Quantity;
				ItemToAdd.SlotIndex = i;
				Inventory.Items.Add(ItemToAdd);
				Inventory.MarkItemDirty(ItemToAdd);
			}
			else
			{
				Inventory.Items[i].Item = Item;
				Inventory.Items[i].ItemQuantity = Quantity;
				Inventory.Items[i].SlotIndex = i;
				Inventory.MarkItemDirty(Inventory.Items[i]);
			}

			GetTDPlayerController()->GetTDHUD()->GetInventoryPanelWidget()->DisplayInventorySlotWidgets();

			return;
		}
	}

	// 빈 슬롯이 없으면 새로운 슬롯 추가
	FInventoryItem ItemToAdd;
	ItemToAdd.Item = Item;
	ItemToAdd.ItemQuantity = Quantity;
	ItemToAdd.SlotIndex = Inventory.Items.Num();

	FInventoryItem& ReferenceToAddedItem = Inventory.Items.Add_GetRef(ItemToAdd);
	Inventory.MarkItemDirty(ReferenceToAddedItem);

	//UTDInventoryDisplayItemObject* Temp = NewObject<UTDInventoryDisplayItemObject>();
	//Temp->Data.Item = Item;
	//Temp->Data.ItemQuantity = Quantity;
	//InventoryDisplayItems.Add(Temp);

	GetTDPlayerController()->GetTDHUD()->GetInventoryPanelWidget()->DisplayInventorySlotWidgets();


	//if (false == GetOwner()->HasAuthority()) // Client가 불린 경우, return
	//{
	//	return;
	//}
}

// 인벤토리 내 해당 아이템이 있는지 확인. Stack 여부 검사 후 쌓기.
// '해당 아이템의 겹쳐진 개수 < 최대 겹침개수'인 경우 true, 아닌 경우 false 리턴.
bool UTDInventoryComponent::FindPartialStack(ATDItemActor* ItemToAdd, FItem& ItemToAddInfo)
{
	//TArray<FInventorySlot>* CategoryArray = nullptr;
	//TArray<UTDInventorySlot*> Category;

	//switch (ItemToAddInfo.ItemCategory)
	//{
	//case EItemCategory::Weapon:
	//	Category = WeaponInventoryCategory;
	//	break;
	//case EItemCategory::Armor:
	//	Category = ArmorInventoryCategory;
	//	break;
	//case EItemCategory::Potion:
	//	Category = PotionInventoryCategory;
	//	break;
	//case EItemCategory::Food:
	//	Category = FoodInventoryCategory;
	//	break;
	//default:
	//	break;
	//}

	////if (Category)
	//{
	//	for (int i = 0; i < Category.Num(); i++)
	//	{
	//		if (UKismetTextLibrary::EqualEqual_TextText(Category[i]->InventorySlot.Item.Name, ItemToAddInfo.Name) &&
	//			Category[i]->InventorySlot.ItemQuantity < ItemToAddInfo.MaxStackSize) // '해당 아이템의 겹쳐진 개수 < 최대 겹침개수'인 경우
	//		{

	//			if (Category[i]->InventorySlot.ItemQuantity + ItemToAdd->GetItemQuantity() <= ItemToAddInfo.MaxStackSize)
	//			{
	//				// 인벤토리에 아이템 추가
	//				AddItemToInventory(ItemToAddInfo, Category[i]->InventorySlot.ItemQuantity + ItemToAdd->GetItemQuantity(), Category, i, CategoryArray);
	//			}
	//			else
	//			{
	//				// 아이템 수량 갱신
	//				int32 RemainingQuantity = FMath::Max(0, ItemToAdd->GetItemQuantity() - (ItemToAddInfo.MaxStackSize - Category[i]->InventorySlot.ItemQuantity));
	//				SetNewItemQuantity(ItemToAdd, RemainingQuantity);

	//				// 인벤토리에 아이템 추가
	//				AddItemToInventory(ItemToAddInfo, Category[i]->InventorySlot.ItemQuantity + (ItemToAdd->GetItemQuantity() - RemainingQuantity), Category, i, CategoryArray);

	//				if (i < Category.Num() - 1 && RemainingQuantity > 0)
	//				{
	//					CreateNewStack(ItemToAdd, ItemToAddInfo);
	//				}
	//			}
	//			
	//			return true;
	//		}
	//	}
	//}

	return false;
}

// Stack 새로 만들기 또는 Stack 불가능한 아이템 채우기.
void UTDInventoryComponent::CreateNewStack(ATDItemActor* ItemToAdd, FItem& ItemToAddInfo)
{
	// 전체 목록에 추가
	AddItemToInventory(ItemToAddInfo, ItemToAdd->GetItemQuantity());

	////****************************************************************************
	////** Category
	//TArray<UTDInventoryDisplayItemObject*> Category;

	//switch (ItemToAddInfo.ItemCategory)
	//{
	//case EItemCategory::Weapon:
	//	Category = WeaponInventoryCategory;
	//	break;
	//case EItemCategory::Armor:
	//	Category = ArmorInventoryCategory;
	//	break;
	//case EItemCategory::Potion:
	//	Category = PotionInventoryCategory;
	//	break;
	//case EItemCategory::Food:
	//	Category = FoodInventoryCategory;
	//	break;
	//default:
	//	break;
	//}
	//
	////if (CategoryArray)
	//{
	//	bool bRelootItem = false;
	//	int32 i = 0; // 루프 외부에서 i를 선언.
	//	for (; i < Category.Num(); i++)
	//	{
	//		if (Category[i]->Data.ItemQuantity == 0)
	//		{
	//			bInventoryIsFull = false;

	//			if (ItemToAdd->GetItemQuantity() > ItemToAddInfo.MaxStackSize)
	//			{
	//				// 아이템 수량 갱신
	//				int32 RemainingQuantity = FMath::Max(0, ItemToAdd->GetItemQuantity() - (ItemToAddInfo.MaxStackSize - Category[i]->Data.ItemQuantity));
	//				SetNewItemQuantity(ItemToAdd, RemainingQuantity);

	//				// 인벤토리에 아이템 추가
	//				AddItemToInventory(ItemToAddInfo, ItemToAddInfo.MaxStackSize, Category, Category[i]->Data.SlotIndex, CategoryArray);
	//				bRelootItem = true;

	//				if (RemainingQuantity == 0)
	//					break;
	//			}
	//			else
	//			{
	//				// 인벤토리에 아이템 추가
	//				AddItemToInventory(ItemToAddInfo, ItemToAdd->GetItemQuantity(), Category, i, CategoryArray);
	//			}

	//			break;
	//		}
	//	}

	//	if (i >= AmountOfSlots)
	//	{
	//		bInventoryIsFull = true;
	//	}

	//	if (bRelootItem)
	//	{
	//		bRelootItem = false;
	//		Server_RelootItem();
	//	}
	//	else
	//	{
	//		if (bInventoryIsFull)
	//		{
	//			// 인벤토리에 아이템 추가
	//			AddItemToInventory(ItemToAddInfo, ItemToAdd->GetItemQuantity(), Category, Category[i]->InventorySlot.SlotIndex, CategoryArray);
	//		}
	//	}
	//}

	////****************************************************************************
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
