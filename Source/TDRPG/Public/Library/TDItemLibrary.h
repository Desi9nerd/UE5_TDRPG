#pragma once
#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "TDItemLibrary.generated.h"

class UTDUW_InventoryEntry;
class UTDUW_InventorySlot;
class ATDItemActor;

/**
 * 
 */

UENUM(BlueprintType)
enum class EItemCategory // 아이템 종류
{
	Weapon,
	Armor,
	Food,
	Potion,
	None
};

// 아이템 종류 및 이미지. DataTable에 필요한 정보
USTRUCT(BlueprintType)
struct FInventoryCategory : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	EItemCategory ItemCategory = EItemCategory::None;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UTexture2D> CategoryIcon;
};

// 아이템 기본정보. DataTable에 필요한 정보.
USTRUCT(BlueprintType)
struct FItem : public FTableRowBase
{
	GENERATED_BODY()

	FItem() : Name(FText::GetEmpty()),
		Thumbnail(nullptr),
		Description(FText::GetEmpty()),
		ItemCategory(EItemCategory::None),
		ItemClass(nullptr),
		bConsumable(false),
		bStackable(false),
		MaxStackSize(0)
	{}

	UPROPERTY(EditDefaultsOnly)
	FText Name;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UTexture2D> Thumbnail;

	UPROPERTY(EditDefaultsOnly)
	FText Description;

	UPROPERTY(EditDefaultsOnly)
	EItemCategory ItemCategory;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ATDItemActor> ItemClass;

	UPROPERTY(EditDefaultsOnly)
	bool bConsumable;

	UPROPERTY(EditDefaultsOnly)
	bool bStackable;

	UPROPERTY(EditDefaultsOnly)
	int32 MaxStackSize;
};

// UI에 보여질 인벤토리 정보
USTRUCT(BlueprintType)
struct FInventoryDisplayItem
{
	GENERATED_BODY()

	FInventoryDisplayItem()
	{
		Item = FItem();
		ItemQuantity = 0;
		SlotIndex = 0;
	}

	UPROPERTY(EditDefaultsOnly)
	FItem Item;

	UPROPERTY(EditDefaultsOnly)
	int32 ItemQuantity;

	UPROPERTY(EditDefaultsOnly)
	int32 SlotIndex;
};

// (UI의 ListView를 사용하기 위해) UObject로 감쌈 
UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class TDRPG_API UTDInventoryDisplayItemObject : public UObject
{
	GENERATED_BODY()

public:
	FInventoryDisplayItem Data;
};

// Runtime
USTRUCT(BlueprintType, Blueprintable)
struct FInventoryItem : public FFastArraySerializerItem
{
	GENERATED_BODY()

	friend struct FInventoryMaster;

	FInventoryItem()
	{
		Item = FItem();
		ItemQuantity = 1;
		SlotIndex = 0;
	}

	UPROPERTY(EditDefaultsOnly)
	FItem Item;

	UPROPERTY(EditDefaultsOnly)
	int32 ItemQuantity;

	UPROPERTY(EditDefaultsOnly)
	int32 SlotIndex;
};

USTRUCT(BlueprintType, Blueprintable)
struct FInventoryMaster : public FFastArraySerializer //Runtime replicated inventory data 
{
	GENERATED_BODY()

	friend struct FInventoryItem;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FInventoryItem> Items;

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FastArrayDeltaSerialize<FInventoryItem>(Items, DeltaParms, *this);
	}
};

template<>
struct TStructOpsTypeTraits<FInventoryMaster> : public TStructOpsTypeTraitsBase2<FInventoryMaster>
{
	enum
	{
		WithNetDeltaSerializer = true,
	};
};


USTRUCT(BlueprintType)
struct FLootItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AActor> LootClass;

	UPROPERTY(EditAnywhere)
	float ChanceToSpawn = 0.f;

	UPROPERTY(EditAnywhere)
	int32 MaxNumberToSpawn = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bLootLevelOverride = true;
};
