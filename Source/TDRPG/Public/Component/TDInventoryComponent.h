#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Library/TDItemLibrary.h"
#include "TDInventoryComponent.generated.h"

class UTDUW_InventorySlot;
class ATDPlayerController;
class ATDItemActor;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TDRPG_API UTDInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UTDInventoryComponent();
	FORCEINLINE int32 GetAmountOfSlots() { return AmountOfSlots; }
	FORCEINLINE EItemCategory& GetSelectedInventoryCategory() { return SelectedInventoryCategory; }
	FORCEINLINE TArray<FInventorySlot>& GetWeaponCategory() { return WeaponCategory; }

	void SetSelectedInventoryCategory(const EItemCategory& InSelectedInventoryCategory);
	UFUNCTION(Client, Reliable) // Client RPC
	void Client_SetSelectedInventoryCategory(const EItemCategory& InSelectedInventoryCategory);

	void PickupItem();
	UFUNCTION(Server, Reliable)
	void Server_PickupItem();

protected:
	virtual void BeginPlay() override;

private:
	void InitializeInventory();
	UFUNCTION(Client, Reliable) // Client RPC
	void Client_InitializeInventory();

	void AddtoInventory(ATDItemActor* InItem);
	UFUNCTION(Client, Reliable) // Client RPC
	void Client_AddtoInventory(ATDItemActor* InItem);

	void AddItemToInventory(FItem Item, int32 Quantity, UTDUW_InventorySlot* InventorySlot, int32 SlotIdx, TArray<FInventorySlot>& OutInventory);

	void FindPartialStack(FItem& ItemToAddInfo);
	void CreateNewStack(FItem& ItemToAddInfo);

	void SetNewItemQuantity(ATDItemActor* InItem, int32 ItemQuantity);
	UFUNCTION(Server, Reliable)
	void Server_SetNewItemQuantity(ATDItemActor* InItem, int32 ItemQuantity);

	void DestroyPickupItem(ATDItemActor* InItem, bool InDestroyItem);
	UFUNCTION(Server, Reliable)
	void Server_DestroyPickupItem(ATDItemActor* InItem, bool InDestroyItem);


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
	
	TObjectPtr<ATDItemActor> ItemToAdd;
	
	bool HasToCreateStack;
};
