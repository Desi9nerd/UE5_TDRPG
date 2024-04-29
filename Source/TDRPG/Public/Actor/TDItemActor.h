#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TDItemActor.generated.h"

class USphereComponent;
class UTexture2D;
class ATDItemActor;

/** 포션, 크리스탈 등의 액터
 *
 */

UENUM(BlueprintType) // InfiniteGameplayEffect 해제 종류
enum class EItemCategory
{
	Inventory,
	None
};

USTRUCT(BlueprintType)
struct FItem : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly)
	FText Name;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UTexture2D> Thumbnail;

	UPROPERTY(EditDefaultsOnly)
	FText Description;

	UPROPERTY(EditDefaultsOnly)
	EItemCategory ItemCategory;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ATDItemActor> ItemClass;

	UPROPERTY(EditDefaultsOnly)
	bool bConsumable;

	UPROPERTY(EditDefaultsOnly)
	bool bStackable;

	UPROPERTY(EditDefaultsOnly)
	int32 MaxStackSize;
};

UCLASS()
class TDRPG_API ATDItemActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ATDItemActor();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> Mesh;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> SphereCollision;

	UPROPERTY(EditDefaultsOnly)
	FString ItemName;
	UPROPERTY(EditDefaultsOnly)
	int32 ItemQuantity;
	UPROPERTY(ReplicatedUsing = OnRep_DestroyItem)
	bool bDestroyItem;

private:
	void SetItemQuantity(int32 InItemQuantity);
	void OnDestroyRootItem(bool InbDestroyItem);

	UFUNCTION()
	void OnRep_DestroyItem();
};
