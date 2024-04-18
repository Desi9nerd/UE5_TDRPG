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

	//static FGameplayTag EquipItemTag;
	//static FGameplayTag EquipNextItemTag;
	//static FGameplayTag UnequipItemTag;
	//static FGameplayTag DropItemTag;

protected:
	void HandleGameplayEvent(FGameplayEventData Payload);

	UFUNCTION(Server, Reliable)
	void ServerHandleGameplayEvent(FGameplayEventData Payload);

	UFUNCTION()
	void AddInventoryTags();


	UPROPERTY(Replicated) // 네트워크 동기화됨.
	FInventoryList InventoryList; // 인벤토리 아이템 목록

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UTDItemStaticData>> DefaultItemStaticDatas; // 기본 아이템 데이터의 클래스. 게임 시작 시 인벤토리에 추가됨. 게임시작과 동시에 캐릭터에 주어지는 아이템만 등록.

	UPROPERTY(Replicated)
	UTDItemInstance* CurrentItem;

	FDelegateHandle ItemTagDelegateHandle;
};
