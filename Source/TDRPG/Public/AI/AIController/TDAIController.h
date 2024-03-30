#pragma once
#include "CoreMinimal.h"
#include "AIController.h"
#include "TDAIController.generated.h"

class UBlackboardComponent;
class UBehaviorTreeComponent;

/**
 * 
 */
UCLASS()
class TDRPG_API ATDAIController : public AAIController
{
	GENERATED_BODY()

public:
	ATDAIController();

protected:
	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;
};
