#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TDInventoryComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TDRPG_API UTDInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UTDInventoryComponent();

protected:
	virtual void BeginPlay() override;

public:	

		
};
