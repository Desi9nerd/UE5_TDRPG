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

	virtual void Tick(float DeltaTime) override;
	void DestroyItem();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnSphereComponentOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void StartSineMovement();
	UFUNCTION()
	void StartRotation();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> Mesh;
	UPROPERTY(EditAnywhere)
	TObjectPtr<USphereComponent> SphereCollision;

	UPROPERTY(EditDefaultsOnly)
	FString ItemName;
	UPROPERTY(EditAnywhere, Replicated)
	int32 ItemQuantity = 1;

	//**********************************************************************
	UPROPERTY(BlueprintReadOnly)
	FVector CalculatedLocation;
	UPROPERTY(BlueprintReadOnly)
	FRotator CalculatedRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup Movement")
	bool bRotates = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup Movement")
	float RotationRate = 45.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup Movement")
	bool bSineMovement = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup Movement")
	float SineAmplitude = 8.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup Movement")
	float SinePeriodConstant = 4.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup Movement")
	FVector InitialLocation;
	//**********************************************************************

private:
	void ItemMovement(float DeltaTime);
	float RunningTime = 0.f;


};
