#pragma once
#include "CoreMinimal.h"
#include "TDItemLibrary.generated.h"

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
