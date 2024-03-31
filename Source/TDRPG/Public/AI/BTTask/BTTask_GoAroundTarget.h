#pragma once
#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlueprintBase.h"
#include "BTTask_GoAroundTarget.generated.h"

/**
 * 
 */
UCLASS()
class TDRPG_API UBTTask_GoAroundTarget : public UBTTask_BlueprintBase
{
	GENERATED_BODY()

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FBlackboardKeySelector NewLocation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FBlackboardKeySelector Target;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Radius;

};
