#pragma once
#include "CoreMinimal.h"
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

USTRUCT(BlueprintType)
struct FInventoryCategory : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	EItemCategory ItemCategory = EItemCategory::None;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UTexture2D> CategoryIcon = nullptr;
};

USTRUCT(BlueprintType)
struct FItem : public FTableRowBase
{
	GENERATED_BODY()

	FItem(){
		Name = FText::GetEmpty();
		Thumbnail = nullptr;
		Description = FText::GetEmpty();
		ItemCategory = EItemCategory::None;
		ItemClass = nullptr;
		bConsumable = false;
		bStackable = false;
		MaxStackSize = 0;
	}

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

USTRUCT(BlueprintType)
struct FInventorySlot
{
	GENERATED_BODY()

	FInventorySlot()
	{
		Item = FItem();
		ItemQuantity = 0;
		InventoryEntry = nullptr;
		SlotIndex = 0;
	}

	UPROPERTY(EditDefaultsOnly)
	FItem Item;

	UPROPERTY(EditDefaultsOnly)
	int32 ItemQuantity;

	UPROPERTY(EditDefaultsOnly)
	UTDUW_InventoryEntry* InventoryEntry;

	UPROPERTY(EditDefaultsOnly)
	int32 SlotIndex;
};

UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class TDRPG_API UTDInventorySlot : public UObject
{
	GENERATED_BODY()

public:
	FInventorySlot InventorySlot;
};

//USTRUCT(BlueprintType, Blueprintable)
//struct FInventoryMaster : public FFastArraySerializer //Runtime replicated inventory data 
//{
//	GENERATED_USTRUCT_BODY()
//
//	friend struct FInventorySlot;
//
//	FInventoryMaster()
//	{
//	}
//
//	UPROPERTY(EditAnywhere, BlueprintReadOnly)
//	TArray<FInventorySlot> Items;
//
//	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
//	{
//		return FastArrayDeltaSerialize<FInventorySlot>(Items, DeltaParms, *this);
//	}
//};
//
//template<>
//struct TStructOpsTypeTraits<FInventoryMaster> : public TStructOpsTypeTraitsBase2<FInventoryMaster> //TStructOpsTypeTraitsBase
//{
//	enum
//	{
//		WithNetDeltaSerializer = true,
//	};
//};


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
