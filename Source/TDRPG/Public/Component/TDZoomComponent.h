#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TDZoomComponent.generated.h"

class USpringArmComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TDRPG_API UTDZoomComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UTDZoomComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void SetZoomValue(float InValue);

protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(EditAnywhere, Category = "Zooming", meta = (AllowPrivateAccess = true))
	float Speed = 20.f;

	UPROPERTY(EditAnywhere, Category = "Zooming", meta = (AllowPrivateAccess = true))
	float InterpSpeed = 5.f;

	UPROPERTY(EditAnywhere, Category = "Zooming", meta = (AllowPrivateAccess = true))
	FVector2D Range = FVector2D(100.f, 1200.f);


	TObjectPtr<USpringArmComponent> SpringArm;

	float CurrentValue;
};
