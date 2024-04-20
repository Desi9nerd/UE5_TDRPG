#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TDComponent.generated.h"

class UImage;

UENUM(BlueprintType)
enum class EItemType : uint8
{
	Consumable	 UMETA(DisplayName = "Consumable"),
	Equipable	 UMETA(DisplayName = "Equipable"),

	Default		UMETA(DisplayName = "Default"),
};

UENUM(BlueprintType)
enum class EItemCategory : uint8
{
	Weapon	 UMETA(DisplayName = "Weapon"),
	Armor	 UMETA(DisplayName = "Armor"),
	Potion	 UMETA(DisplayName = "Potion"),

	Default		UMETA(DisplayName = "Default"),
};

USTRUCT()
struct FItemInfo_Base
{
	GENERATED_BODY()

	FName ItemID;
	FName Title;
	uint8 SerialNumber;
	EItemType ItemType;
	bool bStackable;
	uint8 Quantity;
};

USTRUCT()
struct FItemInfo_UI : public FItemInfo_Base
{
	GENERATED_BODY()

	TObjectPtr<UImage> IconImage;
};

USTRUCT()
struct FItemInfo_TooltipUI : FItemInfo_UI
{
	GENERATED_BODY()

	FItemInfo_Base BaseInfo;
	uint8 Price;
	FString Description;
	uint8 Weight;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TDRPG_API UTDComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UTDComponent();
	UDataTable* GetDataTable(uint8 InItemID);

protected:
	UPROPERTY(EditDefaultsOnly)
	TArray<FItemInfo_UI> IconUI;
	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<FItemInfo_TooltipUI>> TooltipUI;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory|DataTable")
	TObjectPtr<UDataTable> ItemDataTable;
};
