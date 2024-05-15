#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TDDecalActor.generated.h"

UCLASS()
class TDRPG_API ATDDecalActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ATDDecalActor();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UDecalComponent> DecalCircle;
};
