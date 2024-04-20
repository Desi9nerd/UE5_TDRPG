#pragma once
#include "CoreMinimal.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "TDInventoryList.generated.h"

class UTDItemStaticData;
class UTDItemInstance;

/** FFastArraySerializerItem을 상속받아 네트워크 상에서 효율적으로 배열의 변화를 동기화할수있게 한다.
 */
USTRUCT(BlueprintType)
struct FInventoryListItem : public FFastArraySerializerItem
{
	GENERATED_BODY()

public:
	UPROPERTY()
	UTDItemInstance* TDItemInstance = nullptr;
};

/** FFastArraySerializer를 상속받아, 네트워크 동기화를 효율적으로 관리
 */
USTRUCT(BlueprintType)
struct FInventoryList : public FFastArraySerializer
{
	GENERATED_BODY()

public:
	bool NetDeltaSerialize(FNetDeltaSerializeInfo& InDelta);
	void AddItem(TSubclassOf<UTDItemStaticData> InTDItemStaticDataClass);
	void AddItemInstanceOnly(UTDItemInstance* InTDItemInstance);
	void RemoveItem(TSubclassOf<UTDItemStaticData> InTDItemStaticDataClass);

	TArray<FInventoryListItem>& GetInventoryListItemsRef();

protected:
	UPROPERTY()
	TArray<FInventoryListItem> InventoryListItems;
};

// FInventoryList 구조체에 대한 TStructOpsTypeTraits 템플릿을 특수화하여, 이 구조체가 네트워크 동기화를 지원함을 명시함.
// 아래 코드가 없으면 위에서 생성한 FInventoryList이 NetSerialize가 적용되지 않음.
template<>
struct TStructOpsTypeTraits<FInventoryList> : public TStructOpsTypeTraitsBase2<FInventoryList>
{
	enum { WithNetDeltaSerializer = true };
};