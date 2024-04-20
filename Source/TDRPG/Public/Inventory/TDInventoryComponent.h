#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TDInventoryList.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "TDInventoryComponent.generated.h"

/**
 *
 */

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TDRPG_API UTDInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UTDInventoryComponent();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void InitializeComponent() override;
	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	virtual void GameplayEventCallback(const FGameplayEventData* Payload);
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	UTDItemInstance* GetEquippedItem() const;

	UFUNCTION(BlueprintCallable)
	void AddItem(TSubclassOf<UTDItemStaticData> InTDItemStaticDataClass);
	UFUNCTION(BlueprintCallable)
	void AddItemInstanceOnly(UTDItemInstance* InTDItemInstance);
	UFUNCTION(BlueprintCallable)
	void RemoveItem(TSubclassOf<UTDItemStaticData> InTDItemStaticDataClass);
	UFUNCTION(BlueprintCallable)
	void EquipItem(TSubclassOf<UTDItemStaticData> InTDItemStaticDataClass);
	UFUNCTION(BlueprintCallable)
	void EquipItemInstanceOnly(UTDItemInstance* InTDItemInstance);
	UFUNCTION(BlueprintCallable)
	void EquipNextItem();
	UFUNCTION(BlueprintCallable)
	void UnequipItem();
	UFUNCTION(BlueprintCallable)
	void DropItem();
	
protected:
	virtual void BeginPlay() override;
	void HandleGameplayEvent(FGameplayEventData Payload);

	UFUNCTION(Server, Reliable)
	void ServerHandleGameplayEvent(FGameplayEventData Payload);



	UPROPERTY(Replicated) // 네트워크 동기화됨.
	FInventoryList InventoryList; // 인벤토리 아이템 목록

	UPROPERTY(EditDefaultsOnly, ReplicatedUsing = OnRep_InventoryItems)
	TArray<FInventoryList> InventoryItem;

	TMap<FName, TArray<FInventoryList>> InventoryItemsMap;

	UFUNCTION()
	void OnRep_InventoryItems();

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UTDItemStaticData>> DefaultItemStaticDatas; // 게임 시작 시 주어지는 ItemStaticData. 

	UPROPERTY(Replicated)
	UTDItemInstance* CurrentItem;

	FDelegateHandle ItemTagDelegateHandle;

private:
	static UDataTable* GetItemDataTable();
	static bool IsValidItemID(FName InItemID); // Item ID 체크

	static FInventoryListItem GetSelectedItemOverallData(FName InItemID);

	
	////////////////////////////////////////////////////////////
	//** DataTable
	UPROPERTY(EditDefaultsOnly, Category = "Inventory|DataTable")
	TObjectPtr<UDataTable> ItemDataTable;
};
