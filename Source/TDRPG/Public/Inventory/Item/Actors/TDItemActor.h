#pragma once
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "TDItemActor.generated.h"

class UTDItemInstance;
class USphereComponent;

/**
 *
 */

UENUM(BlueprintType)
enum class EItemState : uint8
{
	EIS_None	 UMETA(DisplayName = "None"),
	EIS_Stored	 UMETA(DisplayName = "Stored"),
	EIS_Equipped UMETA(DisplayName = "Equipped"),
	EIS_Dropped  UMETA(DisplayName = "Dropped"),
};

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
	virtual void BeginPlay() override;

	UPROPERTY(ReplicatedUsing = OnRep_TDItemInstance)
	UTDItemInstance* TDItemInstance;

	UFUNCTION()
	void OnRep_TDItemInstance();

	UPROPERTY(ReplicatedUsing = OnRep_ItemState)
	EItemState ItemState = EItemState::EIS_None;
	UFUNCTION()
	void OnRep_ItemState();

	UPROPERTY(EditDefaultsOnly)
	USphereComponent* SphereComponent = nullptr;
	UFUNCTION()
	void OnSphereComponentOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
};
