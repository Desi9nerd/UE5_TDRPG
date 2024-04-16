#pragma once
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "Inventory/Item/TDItemStaticData.h"
#include "TDItemActor.generated.h"

class UTDItemInstance;
class USphereComponent;

/**
 *
 */

UCLASS()
class TDRPG_API ATDItemActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ATDItemActor();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	void InitItemActor(UTDItemInstance* InTDItemInstance);
	virtual void OnEquipped();
	virtual void OnUnequipped();
	virtual void OnDropped();

protected:
	UPROPERTY(ReplicatedUsing = OnRep_TDItemInstance)
	UTDItemInstance* TDItemInstance;

	UFUNCTION()
	void OnRep_TDItemInstance(UTDItemInstance* OldTDItemInstance);

	UPROPERTY(ReplicatedUsing = OnRep_ItemState)
	TEnumAsByte<EItemState> ItemState = EItemState::None;
	UFUNCTION()
	void OnRep_ItemState();

	UPROPERTY()
	USphereComponent* SphereComponent = nullptr;
	UFUNCTION()
	void OnSphereComponentOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	FGameplayTag OverlapEventTag;
};
