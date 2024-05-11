#pragma once
#include "CoreMinimal.h"
#include "TDItemLibrary.generated.h"

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
struct FItem : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	FText Name = FText::FromString(TEXT("None"));

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UTexture2D> Thumbnail = nullptr;

	UPROPERTY(EditDefaultsOnly)
	FText Description = FText::FromString(TEXT("None"));

	UPROPERTY(EditDefaultsOnly)
	EItemCategory ItemCategory = EItemCategory::None;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ATDItemActor> ItemClass = nullptr;

	UPROPERTY(EditDefaultsOnly)
	bool bConsumable = false;

	UPROPERTY(EditDefaultsOnly)
	bool bStackable = false;

	UPROPERTY(EditDefaultsOnly)
	int32 MaxStackSize = 0;
};

USTRUCT(BlueprintType)
struct FInventorySlot
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	FItem Item = FItem();

	UPROPERTY(EditDefaultsOnly)
	int32 ItemQuantity = 0;

	UPROPERTY(EditDefaultsOnly)
	UTDUW_InventorySlot* InventorySlot = nullptr;

	UPROPERTY(EditDefaultsOnly)
	int32 SlotIndex = 0;
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
