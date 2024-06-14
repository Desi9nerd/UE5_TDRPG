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
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess), Category = "Inventory")
	TArray<UTDInventorySlot*> WeaponInventoryCategory;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess), Category = "Inventory")
	TArray<UTDInventorySlot*> ArmorInventoryCategory;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess), Category = "Inventory")
	TArray<UTDInventorySlot*> PotionInventoryCategory;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess), Category = "Inventory")
	TArray<UTDInventorySlot*> FoodInventoryCategory;

	UTDInventoryComponent();
	FORCEINLINE int32 GetAmountOfSlots() { return AmountOfSlots; }
	FORCEINLINE int32 GetAmountOfTotalSlots() { return AmountOfTotalSlots; }
	FORCEINLINE EItemCategory& GetSelectedInventoryCategory() { return SelectedInventoryCategory; }
	//FORCEINLINE TArray<FInventorySlot>& GetWeaponCategory() { return WeaponCategory; }
	//FORCEINLINE TArray<FInventorySlot>& GetArmorCategory() { return ArmorCategory; }
	//FORCEINLINE TArray<FInventorySlot>& GetPotionCategory() { return PotionCategory; }
	//FORCEINLINE TArray<FInventorySlot>& GetFoodCategory() { return FoodCategory; }
	FORCEINLINE TMap<int32, UTDInventorySlot*>& GetAllItems() { return AllItems; }

	void SetSelectedInventoryCategory(const EItemCategory& InSelectedInventoryCategory);
	UFUNCTION(Client, Reliable) // Client RPC
	void Client_SetSelectedInventoryCategory(const EItemCategory& InSelectedInventoryCategory);

	void PickupItem();
	UFUNCTION(Server, Reliable)
	void Server_PickupItem();

	UFUNCTION(Server, Reliable)
	void Server_RelootItem();


	void DropItem(TSubclassOf<ATDItemActor> DropItemClass, int32 ItemQuantity, FVector Location);
	UFUNCTION(Server, Reliable)
	void Server_DropItem(TSubclassOf<ATDItemActor> DropItemClass, int32 ItemQuantity, FVector Location);

	void ConsumeItem(const FText& ItemName);
	UFUNCTION(Server, Reliable)
	void Server_ConsumeItem();


protected:
	virtual void BeginPlay() override;

private:
	void InitializeInventory();
	UFUNCTION(Client, Reliable) // Client RPC
	void Client_InitializeInventory();

	void AddtoInventory(ATDItemActor* InItem);
	UFUNCTION(Client, Reliable) // Client RPC
	void Client_AddtoInventory(ATDItemActor* InItem);

	void AddItemToInventory(const FItem& Item, int32 Quantity, TArray<UTDInventorySlot*> OutInventorySlots, int32 SlotIdx, TArray<FInventorySlot>* OutInventory);

	bool FindPartialStack(ATDItemActor* ItemToAdd, FItem& ItemToAddInfo);
	void CreateNewStack(ATDItemActor* ItemToAdd, FItem& ItemToAddInfo);

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
	int32 AmountOfTotalSlots = 64;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess), Category = "Inventory")
	EItemCategory SelectedInventoryCategory;

	

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess), Category = "Inventory")
	TMap<int32, UTDInventorySlot*> AllItems;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess), Category = "Inventory")
	TObjectPtr<UDataTable> ItemDataTable;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess), Category = "Inventory")
	TObjectPtr<USoundBase> Sound_DestroyLootedItem;

	bool HasToCreateStack;
	//bool bRelootItem = false;
	bool bInventoryIsFull = false;
};
