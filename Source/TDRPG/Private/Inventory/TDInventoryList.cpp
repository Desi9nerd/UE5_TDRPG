#include "Inventory/TDInventoryList.h"
#include "Inventory/Item/TDItemInstance.h"
#include "Inventory/Item/TDItemStaticData.h"
#include "Net/Serialization/FastArraySerializer.h"

// 네트워크를 통해 아이템 리스트의 변화를 동기화하는 역할.
// FFastArraySerializer의 FastArrayDeltaSerialize 함수를 호출하여 실제 동기화 작업을 수행함.
// FInventoryListItem 타입의 아이템과 FInventoryList 인벤토리 리스트 간의 변화를 처리함.
bool FInventoryList::NetDeltaSerialize(FNetDeltaSerializeInfo& InDelta)
{
	return FFastArraySerializer::FastArrayDeltaSerialize<FInventoryListItem, FInventoryList>(InventoryListItems, InDelta, *this);
}

// 아이템을 인벤토리에 추가
void FInventoryList::AddItem(TSubclassOf<UTDItemStaticData> InTDItemStaticDataClass)
{
	FInventoryListItem& ItemInfo = InventoryListItems.AddDefaulted_GetRef();
	ItemInfo.TDItemInstance = NewObject<UTDItemInstance>();
	ItemInfo.TDItemInstance->Init(InTDItemStaticDataClass);

	MarkItemDirty(ItemInfo); // 특정 아이템(ItemInfo)이 변경되었음을 시스템에 알림.
}

void FInventoryList::AddItemInstanceOnly(UTDItemInstance* InTDItemInstance)
{
	FInventoryListItem& ItemInfo = InventoryListItems.AddDefaulted_GetRef();
	ItemInfo.TDItemInstance = InTDItemInstance;

	MarkItemDirty(ItemInfo);
}

// 아이템을 인벤토리에서 제거
void FInventoryList::RemoveItem(TSubclassOf<UTDItemStaticData> InTDItemStaticDataClass)
{
	// 리스트를 순회하여 해당 타입의 아이템을 찾아 제거.
	for (int32 i = 0; i < InventoryListItems.Num(); i++)
	{
		FInventoryListItem& Item = InventoryListItems[i];
		if (Item.TDItemInstance && Item.TDItemInstance->GetTDItemStaticData()->IsA(InTDItemStaticDataClass))
		{
			InventoryListItems.RemoveAt(i); // 해당 i의 아이템을 제거
			MarkArrayDirty();				// 배열 자체가 변경되었음을 시스템에 알림.
			break;							// 아이템을 찾아 제거한 후 루프를 종료
		}
	}
}

TArray<FInventoryListItem>& FInventoryList::GetInventoryListItemsRef()
{
	return InventoryListItems;
}


//**************************************************************************************
/* 부연설명
 * MarkItemDirty();
 * 이 함수는 FFastArraySerializerItem을 상속받는 FInventoryListItem의 인스턴스인 ItemInfo에 대해 호출된다. 이 함수를 호출함으로써, 해당 아이템에 대한 변경 사항이 네트워크를 통해 복제 대상에게 전송되어야 함을 언리얼 엔진의 네트워크 시스템에 알린다. 인벤토리 아이템이 추가되거나 수정되었을때, 해당 아이템의 최신 상태가 네트워크를 통해 다른 클라이언트나 서버와 동기화되도록 한다.
 *
 * MarkArrayDirty();
 * 이 함수는 FInventoryList 구조체 내에서 호출된다. FInventoryList가 상속받는 FFastArraySerializer 클래스에 정의된 함수다. 배열에 아이템이 추가되거나 제거되는 경우와 같이, 배열의 구조(ex.배열에 포함된 아이템의 개수나 배열의 순서 등)가 변경될 때 이 함수를 호출한다. 이렇게 함으로써, 배열 전체에 대한 변경 사항이 네트워크를 통해 복제되어야 함을 언리얼 엔진에 알린다. 결과적으로 인벤토리 리스트의 구조적 변경(ex.아이템의 추가나 제거)이 다른 클라이언트나 서버와 동기화된다.
 *
 */
 //**************************************************************************************