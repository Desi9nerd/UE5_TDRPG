#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Library/TDItemLibrary.h"
#include "TDInventoryComponent.generated.h"

class ATDPlayerController;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TDRPG_API UTDInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UTDInventoryComponent();
	FORCEINLINE int32 GetAmountOfSlots() { return AmountOfSlots; }

	void SetSelectedInventoryCategory(const EItemCategory& InSelectedInventoryCategory);
	UFUNCTION(Client, Reliable) // Client RPC
	void Client_SetSelectedInventoryCategory(const EItemCategory& InSelectedInventoryCategory);

protected:
	virtual void BeginPlay() override;

private:
	void InitializeInventory();
	UFUNCTION(Client, Reliable) // Client RPC
	void Client_InitializeInventory();



	UPROPERTY()
	TObjectPtr<ATDPlayerController> TDPlayerController;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess), Category = "Inventory")
	int32 AmountOfSlots = 16;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess), Category = "Inventory")
	EItemCategory SelectedInventoryCategory;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess), Category = "Inventory")
	TArray<FInventorySlot> WeaponCategory;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess), Category = "Inventory")
	TArray<FInventorySlot> ArmorCategory;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess), Category = "Inventory")
	TArray<FInventorySlot> PotionCategory;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess), Category = "Inventory")
	TArray<FInventorySlot> FoodCategory;
	
};
