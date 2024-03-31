#include "AI/BTService/BTService_SearchPlayer.h"
#include "AIController.h"

void UBTService_SearchPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// BTService_BlueprintBase에 AIOwner, ActorOwner 변수가 존재함.
	/** TObjectPtr<AAIController> AIOwner; -  Cached AIController owner of BehaviorTreeComponent. */
	/** TObjectPtr<AActor> ActorOwner - Cached actor owner of BehaviorTreeComponent. */
	GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, *AIOwner->GetName());
	GEngine->AddOnScreenDebugMessage(2, 1.f, FColor::Green, *ActorOwner->GetName());
}
