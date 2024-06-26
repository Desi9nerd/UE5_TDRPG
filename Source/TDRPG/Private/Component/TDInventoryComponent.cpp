﻿#include "Component/TDInventoryComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Actor/TDItemActor.h"
#include "Character/TDCharacter.h"
#include "GameFramework/Character.h"
#include "GAS/Data/TDDA_ItemGE.h"
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
			//InventoryDisplayItems.Add(ItemToAdd);
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
	checkf(ItemDataTable, TEXT("No Item DataTable. Check UTDInventoryComponent::Client_AddtoInventory_Implementation"));
	if (ItemDataTable)
	{
		FItem ItemToAddInfo = *(ItemDataTable->FindRow<FItem>(FName(*(InItem->GetItemName())), FString("")));
		
		if (ItemToAddInfo.bStackable) // Stack 가능한 아이템이라면
		{
			FindPartialStackAndStore(InItem, ItemToAddInfo);	// 인벤토리 내에 해당 아이템이 있는지 찾은 후 아이템 추가.
		}
		else
		{
			if (Inventory.Items.Num() >= GetAmountOfSlots()) { // 인벹토리가 꽉 찬 경우
				UE_LOG(LogTemp, Log, TEXT("Inventory is Full!"));
				return;
			}
			CreateNewSlotAndStore(InItem, ItemToAddInfo);
		}
	}
}

// 인벤토리 내 해당  Stackable 아이템이 있는지 확인 후 적재.
void UTDInventoryComponent::FindPartialStackAndStore(ATDItemActor* ItemToAdd, FItem& ItemToAddInfo)
{
	//************************************************************************************************
	//** 해당 이름의 아이템을 발견 && '해당 아이템의 겹쳐진 개수 > 0' && '해당 아이템의 겹쳐진 개수 < 최대 겹침개수'인 경우 **//
	for (int i = 0; i < InventoryDisplayItems.Num(); i++)
	{		
		bool bStatckable = UKismetTextLibrary::EqualEqual_TextText(InventoryDisplayItems[i]->Data.Item.Name, ItemToAddInfo.Name)
							&& InventoryDisplayItems[i]->Data.ItemQuantity > 0
							&& InventoryDisplayItems[i]->Data.ItemQuantity < ItemToAddInfo.MaxStackSize;
		if (bStatckable)
		{
			//* '현재 아이템의 수량 + 추가하는 수량 <= MaxStackSize'인 경우 *//
			if (InventoryDisplayItems[i]->Data.ItemQuantity + ItemToAdd->GetItemQuantity() <= ItemToAddInfo.MaxStackSize)
			{
				// 현재 존재하는 InventoryDisplayItems[i]에 아이템 추가하여 수량 변경
				AddItemToInventory(ItemToAddInfo, InventoryDisplayItems[i]->Data.ItemQuantity + ItemToAdd->GetItemQuantity(), i);
			}
			//* '현재 아이템의 수량 + 추가하는 수량 > MaxStackSize'인 경우 *//
			else
			{
				// 아이템 수량 업데이트
				int32 RemainingQuantity = FMath::Max(0, ItemToAdd->GetItemQuantity() - (ItemToAddInfo.MaxStackSize - InventoryDisplayItems[i]->Data.ItemQuantity));

				// 현재 존재하는 InventoryDisplayItems[i]에 아이템 추가하여 최대수량으로 변경
				AddItemToInventory(ItemToAddInfo, ItemToAddInfo.MaxStackSize, i);
				
				while (i < InventoryDisplayItems.Num() - 1 && RemainingQuantity > 0)
				{
					if (RemainingQuantity <= ItemToAddInfo.MaxStackSize)
					{
						AddItemToInventory(ItemToAddInfo, RemainingQuantity, Inventory.Items.Num());
						RemainingQuantity = 0;
						break;
					}

					AddItemToInventory(ItemToAddInfo, ItemToAddInfo.MaxStackSize, Inventory.Items.Num());
					RemainingQuantity -= ItemToAddInfo.MaxStackSize;
					i++;
				}

				// 인벤토리가 꽉 차서 아이템 수량을 다 채울 수 없는 경우, 아이템 수량 갱신
				if (RemainingQuantity > 0) 
				{
					SetNewItemQuantity(ItemToAdd, RemainingQuantity);
					return;
				}
			}

			DestroyPickupItem(ItemToAdd, true);
			UGameplayStatics::PlaySound2D(this, Sound_DestroyLootedItem);
			return;
		}
	}
	//************************************************************************************************

	//************************************************************************************************
	//** 겹침이 해당되지 않는 경우, 새로운 인벤토리 슬롯을 생성 후 적재 **//
	for (int32 i = 0; i < InventoryDisplayItems.Num(); i++)
	{
		if (InventoryDisplayItems[i]->Data.ItemQuantity == 0)
		{
			//* '현재 아이템의 수량 + 추가하는 수량 <= MaxStackSize'인 경우 *//
			if (ItemToAdd->GetItemQuantity() <= ItemToAddInfo.MaxStackSize)
			{
				// 현재 존재하는 InventoryDisplayItems[i]에 아이템 추가하여 수량 변경
				AddItemToInventory(ItemToAddInfo, ItemToAdd->GetItemQuantity(), i);
			}
			//* '현재 아이템의 수량 + 추가하는 수량 > MaxStackSize'인 경우 *//
			else
			{
				// 아이템 수량 업데이트
				int32 RemainingQuantity = FMath::Max(0, ItemToAdd->GetItemQuantity() - ItemToAddInfo.MaxStackSize);

				// 현재 존재하는 InventoryDisplayItems[i]에 아이템 추가하여 최대수량으로 변경
				AddItemToInventory(ItemToAddInfo, ItemToAddInfo.MaxStackSize, i);
				
				while (i < InventoryDisplayItems.Num() - 1 && RemainingQuantity > 0)
				{
					if (RemainingQuantity <= ItemToAddInfo.MaxStackSize)
					{
						AddItemToInventory(ItemToAddInfo, RemainingQuantity, Inventory.Items.Num());
						RemainingQuantity = 0;
						break;
					}

					AddItemToInventory(ItemToAddInfo, ItemToAddInfo.MaxStackSize, Inventory.Items.Num());
					RemainingQuantity -= ItemToAddInfo.MaxStackSize;
					i++;
				}

				// 인벤토리가 꽉 차서 아이템 수량을 다 채울 수 없는 경우, 아이템 수량 갱신
				if (RemainingQuantity > 0)
				{
					SetNewItemQuantity(ItemToAdd, RemainingQuantity);
					return;
				}
			}

			DestroyPickupItem(ItemToAdd, true);
			UGameplayStatics::PlaySound2D(this, Sound_DestroyLootedItem);
			return;
		}
	}
	//************************************************************************************************
}

// Stack 불가능한 아이템 채우기.
void UTDInventoryComponent::CreateNewSlotAndStore(ATDItemActor* ItemToAdd, FItem& ItemToAddInfo)
{	
	for (int32 i = 0; i < InventoryDisplayItems.Num(); i++)
	{
		if (InventoryDisplayItems[i]->Data.ItemQuantity == 0)
		{
			// 인벤토리에 아이템 추가
			AddItemToInventory(ItemToAddInfo, ItemToAdd->GetItemQuantity(), i);
			break;
		}
	}

	DestroyPickupItem(ItemToAdd, true);
	UGameplayStatics::PlaySound2D(this, Sound_DestroyLootedItem);
}

void UTDInventoryComponent::AddItemToInventory(const FItem& Item, int32 Quantity, int32 SlotIdx)
{
	InventoryDisplayItems[SlotIdx]->Data.Item = Item;
	InventoryDisplayItems[SlotIdx]->Data.ItemQuantity = Quantity;
	InventoryDisplayItems[SlotIdx]->Data.SlotIndex = SlotIdx;
	
	if (Inventory.Items.Num() <= SlotIdx)
	{
		FInventoryItem ItemToAdd;
		ItemToAdd.Item = Item;
		ItemToAdd.ItemQuantity = Quantity;
		ItemToAdd.SlotIndex = SlotIdx;
		Inventory.Items.Add(ItemToAdd);
		Inventory.MarkItemDirty(ItemToAdd);
	}
	else
	{
		Inventory.Items[SlotIdx].Item = Item;
		Inventory.Items[SlotIdx].ItemQuantity = Quantity;
		Inventory.Items[SlotIdx].SlotIndex = SlotIdx;
		Inventory.MarkItemDirty(Inventory.Items[SlotIdx]);
	}

	GetTDPlayerController()->GetTDHUD()->GetInventoryPanelWidget()->DisplayInventorySlotWidgets();
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
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner());
	if (nullptr == ASC) return;
	checkf(DA_ItemGE, TEXT("No DA_ItemGE. Assign DataAsset at UTDInventoryComponent::ConsumeItem()"));

	for (TTuple<FName, TSubclassOf<UGameplayEffect>> Iter : DA_ItemGE->ItemGEClassList)
	{
		if (Iter.Key == FName(*((ItemName).ToString())))
		{
			FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
			EffectContextHandle.AddSourceObject(this);
			const FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(Iter.Value, 1.f, EffectContextHandle);
			ASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
		}
	}
}

void UTDInventoryComponent::Server_ConsumeItem_Implementation(const FText& ItemName)
{
	
}
//******************************************************************************
