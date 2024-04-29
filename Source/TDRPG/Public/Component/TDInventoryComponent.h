#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Library/TDItemLibrary.h"
#include "TDInventoryComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TDRPG_API UTDInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UTDInventoryComponent();

protected:
	virtual void BeginPlay() override;

private:
	void InitializeInventory();
	UFUNCTION(Client, Reliable) // Client RPC
	void Client_InitializeInventory();


	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess), Category = "Inventory")
	int32 AmountOfSlots;

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
