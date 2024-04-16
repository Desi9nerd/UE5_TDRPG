#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TDInventoryList.h"
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

protected:
	UPROPERTY(Replicated) // 네트워크 동기화됨.
	FInventoryList InventoryList; // 인벤토리 아이템 목록

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UTDItemStaticData>> DefaultItemStaticDatas; // 기본 아이템 데이터의 클래스. 게임 시작 시 인벤토리에 추가됨.
};
