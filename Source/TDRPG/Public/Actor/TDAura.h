

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TDAura.generated.h"

UCLASS()
class TDRPG_API ATDAura : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATDAura();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
