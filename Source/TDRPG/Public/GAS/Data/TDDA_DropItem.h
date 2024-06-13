#pragma once
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Library/TDItemLibrary.h"
#include "TDDA_DropItem.generated.h"

/**
 * 
 */
UCLASS()
class TDRPG_API UTDDA_DropItem : public UDataAsset
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	TArray<FLootItem> GetLootItems();

	UPROPERTY(EditDefaultsOnly, Category = "LootTiers|Spawning")
	TArray<FLootItem> LootItems;
};
