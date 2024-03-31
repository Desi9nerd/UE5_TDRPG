#include "AI/BTTask/BTTask_GoAroundTarget.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "NavigationSystem.h"

EBTNodeResult::Type UBTTask_GoAroundTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AActor* TargetActor = UBTFunctionLibrary::GetBlackboardValueAsActor(this, Target);
	if (IsValid(TargetActor))
	{
		FVector RandomLocation;

		bool bSuccess = UNavigationSystemV1::K2_GetRandomLocationInNavigableRadius(
			this,
			TargetActor->GetActorLocation(),
			RandomLocation,
			Radius
		);

		UBTFunctionLibrary::SetBlackboardValueAsVector(this, NewLocation, RandomLocation); // NewLocation을 RandomLocation로 업데이트.

		if (bSuccess) return EBTNodeResult::Succeeded; // 성공 종료
	}
	
	return EBTNodeResult::Failed;
}
