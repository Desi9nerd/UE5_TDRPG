#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TDItemActor.generated.h"

class USphereComponent;
class UTexture2D;

/** 포션, 크리스탈 등의 액터
 * 
 */

UCLASS()
class TDRPG_API ATDItemActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ATDItemActor();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	FORCEINLINE const FString& GetItemName() const { return ItemName; }
	FORCEINLINE int32 GetItemQuantity() const { return ItemQuantity; }
	FORCEINLINE void SetItemQuantity(int32 InItemQuantity) { ItemQuantity = InItemQuantity; }
	//FORCEINLINE void DestroyPickupItem(bool DestroyItem) { bDestroyItem = DestroyItem; } // 삭제예정: 옵션1
	void DestroyItem(); // 위에거 대체용으로 만든 함수

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnSphereComponentOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> Mesh;
	UPROPERTY(EditAnywhere)
	TObjectPtr<USphereComponent> SphereCollision;


private:
	void OnDestroyRootItem(bool InbDestroyItem);

	//UFUNCTION()
	//void OnRep_DestroyItem(); // 삭제예정: 옵션1

	UPROPERTY(EditDefaultsOnly)
	FString ItemName;

	UPROPERTY(EditDefaultsOnly, Replicated)
	int32 ItemQuantity;

	//UPROPERTY(ReplicatedUsing = OnRep_DestroyItem) // 삭제예정: 옵션1
	//bool bDestroyItem;
};
