#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TDItemActor.generated.h"

class UTDItemInstance;

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
};
