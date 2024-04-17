#include "Inventory/TDInventoryComponent.h"
#include "Net/UnrealNetwork.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagsManager.h"
#include "Engine/ActorChannel.h"
#include "Inventory/Item/TDItemInstance.h"
#include "Inventory/Item/TDItemStaticData.h"
#include "Inventory/Item/Actors/TDItemActor.h"

#include "Abilities/GameplayAbilityTypes.h"

FGameplayTag UTDInventoryComponent::EquipItemTag;
FGameplayTag UTDInventoryComponent::EquipNextItemTag;
FGameplayTag UTDInventoryComponent::UnequipItemTag;
FGameplayTag UTDInventoryComponent::DropItemTag;


static TAutoConsoleVariable<int32> ConsoleVarShowInventory( // 디버깅용 콘솔 변수
	TEXT("ShowDebugInventory"),
	0,
	TEXT("Draws debug - Inventory Info")
	TEXT(" 0: off/n")
	TEXT(" 1: on/n"),
	ECVF_Cheat
);

UTDInventoryComponent::UTDInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;   // 매 틱마다 업데이트 가능하도록 설정
	bWantsInitializeComponent = true;           // InitializeComponent() 함수 호출을 원함
	SetIsReplicatedByDefault(true); // 아래 내용 참조.

	static bool bHandledAddingTags = false;
	if (false == bHandledAddingTags)
	{
		UGameplayTagsManager::Get().OnLastChanceToAddNativeTags().AddUObject(this, &ThisClass::AddInventoryTags);		
	}
}

void UTDInventoryComponent::AddInventoryTags()
{
	UGameplayTagsManager& TagsManager = UGameplayTagsManager::Get();

	UTDInventoryComponent::EquipItemTag = TagsManager.AddNativeGameplayTag(TEXT("Event.Inventory.EquipItem"), TEXT("이벤트: 아이템 장착"));
	UTDInventoryComponent::EquipNextItemTag = TagsManager.AddNativeGameplayTag(TEXT("Event.Inventory.EquipNextItem"), TEXT("이벤트: 다음 아이템 장착"));
	UTDInventoryComponent::UnequipItemTag = TagsManager.AddNativeGameplayTag(TEXT("Event.Inventory.UnequipItem"), TEXT("이벤트: 아이템 장착해제"));
	UTDInventoryComponent::DropItemTag = TagsManager.AddNativeGameplayTag(TEXT("Event.Inventory.DropItem"), TEXT("이벤트: 아이템 떨어뜨리기"));

	TagsManager.OnLastChanceToAddNativeTags().RemoveAll(this); // UnSubscribe
}

void UTDInventoryComponent::HandleGameplayEvent(FGameplayEventData Payload)
{
	ENetRole NetRole = GetOwnerRole();

	if (NetRole == ROLE_Authority)
	{
		FGameplayTag EventTag = Payload.EventTag;

		if (EventTag == UTDInventoryComponent::EquipItemTag)
		{
			const UTDItemInstance* ItemInstance = Cast<UTDItemInstance>(Payload.OptionalObject);
			if (IsValid(ItemInstance))
			{
				AddItemInstanceOnly(const_cast<UTDItemInstance*>(ItemInstance));

				if (Payload.Instigator)
				{
					const_cast<AActor*>(Payload.Instigator.Get())->Destroy();
				}
			}
		}
		else if (EventTag == UTDInventoryComponent::EquipNextItemTag) { EquipNextItem(); }
		else if (EventTag == UTDInventoryComponent::UnequipItemTag) { UnequipItem(); }
		else if (EventTag == UTDInventoryComponent::DropItemTag) { DropItem(); }
	}
}

void UTDInventoryComponent::ServerHandleGameplayEvent_Implementation(FGameplayEventData Payload)
{
	HandleGameplayEvent(Payload);
}

void UTDInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UTDInventoryComponent, InventoryList);
	DOREPLIFETIME(UTDInventoryComponent, CurrentItem);
}

void UTDInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	const bool bShowDebug = (ConsoleVarShowInventory.GetValueOnGameThread() != 0);
	if (bShowDebug)
	{
		// 인벤토리 아이템 목록을 순회하며 디버그 정보 출력
		for (FInventoryListItem& Item : InventoryList.GetInventoryListItemsRef())
		{
			UTDItemInstance* ItemInstance = Item.TDItemInstance;
			const UTDItemStaticData* ItemStaticData = ItemInstance->GetTDItemStaticData();

			if (IsValid(ItemInstance) && IsValid(ItemStaticData))
			{
				GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Magenta, FString::Printf(TEXT("Item: %s"), *ItemStaticData->Name.ToString())); // 아이템 이름을 화면에 출력
			}
		}
	}
}

void UTDInventoryComponent::InitializeComponent() // 초기화. 서버에서만 실행.
{
	Super::InitializeComponent();

	if (GetOwner()->HasAuthority()) // Server
	{
		// DefaultItemStaticDatas 배열에 있는 모든 아이템을 InventoryList에 추가
		for (const TSubclassOf<UTDItemStaticData>& ItemStaticDataClass : DefaultItemStaticDatas)
		{
			InventoryList.AddItem(ItemStaticDataClass);
		}
	}

	if (UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner()))
	{
		ItemTagDelegateHandle = ASC->GenericGameplayEventCallbacks.FindOrAdd(UTDInventoryComponent::EquipItemTag).AddUObject(this, &ThisClass::GameplayEventCallback);
		ItemTagDelegateHandle = ASC->GenericGameplayEventCallbacks.FindOrAdd(UTDInventoryComponent::EquipNextItemTag).AddUObject(this, &ThisClass::GameplayEventCallback);
		ItemTagDelegateHandle = ASC->GenericGameplayEventCallbacks.FindOrAdd(UTDInventoryComponent::UnequipItemTag).AddUObject(this, &ThisClass::GameplayEventCallback);
		ItemTagDelegateHandle = ASC->GenericGameplayEventCallbacks.FindOrAdd(UTDInventoryComponent::DropItemTag).AddUObject(this, &ThisClass::GameplayEventCallback);
	}
}

// 네트워크 동기화를 위한 함수. 인벤토리 내의 각 아이템 인스턴스도 네트워크를 통해 복제되도록 함.
// 멀티플레이어에서 다른 플레이어들도 해당 아이템의 상태를 볼 수 있도록 하는 데 필요함.
bool UTDInventoryComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool bRenewed = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (const FInventoryListItem& Item : InventoryList.GetInventoryListItemsRef()) // InventoryList 내의 모든 아이템을 순회
	{
		UTDItemInstance* ItemInstance = Item.TDItemInstance;  // 각 아이템 인스턴스를 가져옴.

		if (IsValid(ItemInstance))
		{
			// Channel을 통해 ItemInstance를 복제. 네트워크를 통해 해당 ItemInstance의 상태가 클라이언트에 동기화되도록 함.
			bRenewed |= Channel->ReplicateSubobject(ItemInstance, *Bunch, *RepFlags);
		}
	}

	return bRenewed; // 복제가 성공적으로 이루어졌는지 여부를 반환
}

void UTDInventoryComponent::GameplayEventCallback(const FGameplayEventData* Payload)
{
	ENetRole NetRole = GetOwnerRole();

	if (NetRole == ROLE_Authority) // Server
	{
		HandleGameplayEvent(*Payload);
	}
	else if (NetRole == ROLE_AutonomousProxy) // Client
	{
		ServerHandleGameplayEvent(*Payload);
	}
}

UTDItemInstance* UTDInventoryComponent::GetEquippedItem() const
{
	return CurrentItem;
}

void UTDInventoryComponent::AddItem(TSubclassOf<UTDItemStaticData> InTDItemStaticDataClass)
{
	if (GetOwner()->HasAuthority()) // Server
	{
		InventoryList.AddItem(InTDItemStaticDataClass);
	}
}

void UTDInventoryComponent::AddItemInstanceOnly(UTDItemInstance* InTDItemInstance)
{
	if (GetOwner()->HasAuthority()) // Server
	{
		InventoryList.AddItemInstanceOnly(InTDItemInstance);
	}
}

void UTDInventoryComponent::RemoveItem(TSubclassOf<UTDItemStaticData> InTDItemStaticDataClass)
{
	InventoryList.RemoveItem(InTDItemStaticDataClass);
}

void UTDInventoryComponent::EquipItem(TSubclassOf<UTDItemStaticData> InTDItemStaticDataClass)
{
	if (GetOwner()->HasAuthority()) // Server
	{
		for (const FInventoryListItem& Item: InventoryList.GetInventoryListItemsRef())
		{
			if (Item.TDItemInstance->TDItemStaticDataClass == InTDItemStaticDataClass)
			{
				Item.TDItemInstance->OnEquipped(GetOwner());
				CurrentItem = Item.TDItemInstance; // 현재 아이템을 장착하는 아이템으로 설정.

				break;
			}
		}

		DropItem();
	}
}

void UTDInventoryComponent::EquipItemInstanceOnly(UTDItemInstance* InTDItemInstance)
{
	if (GetOwner()->HasAuthority()) // Server
	{
		for (const FInventoryListItem& Item : InventoryList.GetInventoryListItemsRef())
		{
			if (Item.TDItemInstance == InTDItemInstance) // 해당 TDItemInstance를 찾으면
			{
				Item.TDItemInstance->OnEquipped(GetOwner());
				CurrentItem = Item.TDItemInstance; // 현재 아이템을 장착하는 아이템으로 설정.

				break;
			}
		}

		DropItem();
	}
}

void UTDInventoryComponent::EquipNextItem()
{
	TArray<FInventoryListItem>& InventoryListItems = InventoryList.GetInventoryListItemsRef();

	const bool bNoItems = InventoryListItems.Num() == 0;
	const bool bOneAndEquipped = InventoryListItems.Num() == 1 && CurrentItem;

	if (bNoItems || bOneAndEquipped) return;

	UTDItemInstance* NewEquipItem = CurrentItem;

	for (const FInventoryListItem& Item : InventoryListItems)
	{
		if (Item.TDItemInstance->GetTDItemStaticData()->bCanBeEquipped)
		{
			if (Item.TDItemInstance != CurrentItem)
			{
				NewEquipItem = Item.TDItemInstance; // 새롭게 장착할 아이템
				break;
			}
		}
	}

	if (CurrentItem)
	{
		if (NewEquipItem == CurrentItem) // 새롭게 장착할 아이템을 못 찾아 NewEquipItem == CurrentItem인 경우
		{
			return;
		}

		UnequipItem();
	}

	EquipItemInstanceOnly(NewEquipItem); // 아이템 장착
}

void UTDInventoryComponent::UnequipItem()
{
	if (GetOwner()->HasAuthority()) // Server
	{
		if (IsValid(CurrentItem))
		{
			CurrentItem->OnUnequipped(GetOwner());
			CurrentItem = nullptr; // 현재 아이템을 nullptr로 설정.
		}
	}
}

void UTDInventoryComponent::DropItem()
{
	if (GetOwner()->HasAuthority()) // Server
	{
		if (IsValid(CurrentItem))
		{
			CurrentItem->OnDropped(GetOwner());
			RemoveItem(CurrentItem->TDItemStaticDataClass);
			CurrentItem = nullptr; // 현재 아이템을 nullptr로 설정.
		}
	}
}



/**************************************************************************************
/* 부연설명
 * SetIsReplicated() vs. SetIsReplicatedByDefault()
 * SetIsReplicated()는 실행 시간에 특정 인스턴스의 복제를 제어하는데 사용되고, SetIsReplicatedByDefault()는 컴포넌트가 처음 생성될 때부터 복제될지 여부를 클래스 레벨에서 설정하는 데 사용된다. 
 * 게임의 동적인 요소를 제어하려면 SetIsReplicated()를, 모든 인스턴스에 대한 기본 설정을 정의하려면 SetIsReplicatedByDefault()를 사용하는 것이 좋다.
 */
 //**************************************************************************************