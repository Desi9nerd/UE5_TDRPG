#pragma once
#include "CoreMinimal.h"
#include "Inventory/TDInventoryList.h"
#include "UObject/NoExportTypes.h"
#include "TDItemInstance.generated.h"

enum class EItemState : uint8;
class UTDItemStaticData;
class UImage;

/**
 * 
 */

UCLASS(BlueprintType, Blueprintable)
class TDRPG_API UTDItemInstance : public UObject
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const;
	virtual bool IsSupportedForNetworking() const override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName ItemID;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	uint8 SerialNumber;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName ItemType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bStackable = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	uint8 Quantity;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UImage> IconImage;
};
