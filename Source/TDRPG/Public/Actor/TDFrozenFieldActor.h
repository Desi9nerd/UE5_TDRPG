#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TDFrozenFieldActor.generated.h"

UCLASS()
class TDRPG_API ATDFrozenFieldActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ATDFrozenFieldActor();

	UFUNCTION(BlueprintPure)
	USceneComponent* GetGroundPoint(const FVector& GroundLocation, int32 NumPoints, float YawOverride = 0.f);

protected:

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TObjectPtr<USceneComponent> FrozenField;
};
