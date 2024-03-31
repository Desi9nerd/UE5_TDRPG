#include "AI/BTService/BTService_SearchPlayer.h"
#include "AIController.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

void UBTService_SearchPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// BTService_BlueprintBase에 AIOwner, ActorOwner 변수가 존재함.
	/** TObjectPtr<AAIController> AIOwner; -  Cached AIController owner of BehaviorTreeComponent. */
	/** TObjectPtr<AActor> ActorOwner - Cached actor owner of BehaviorTreeComponent. */

	//GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, *AIOwner->GetName());
	//GEngine->AddOnScreenDebugMessage(2, 1.f, FColor::Green, *ActorOwner->GetName());

	APawn* OwningPawn = AIOwner->GetPawn();

	// BP_Player와 BP_Enemy에서 지정해준 Actor Tag 사용.
	// Player 입장에서는 TargetTag는 "Enemy", Enemy입장에서는 TargetTag는 "Player".
	const FName TargetTag = OwningPawn->ActorHasTag(FName("Player")) ? FName("Enemy") : FName("Player");

	TArray<AActor*> ActorsWithTag;
	UGameplayStatics::GetAllActorsWithTag(OwningPawn, TargetTag, ActorsWithTag); // TargetTag와 일치하는 Actor Tag를 가진 모든 Pawn들을 ActorsWithTag 액터배열에 담는다.

	float ClosestDistance = TNumericLimits<float>::Max(); // 최대값을 기본값으로 설정.
	AActor* ClosestActor = nullptr; // 가장 가까운 Actor
	for (AActor* Actor : ActorsWithTag)
	{
		// GEngine->AddOnScreenDebugMessage(-1, .5f, FColor::Blue, *Actor->GetName());

		if (IsValid(Actor) && IsValid(OwningPawn))
		{
			const float Distance = OwningPawn->GetDistanceTo(Actor);
			if (Distance < ClosestDistance)
			{
				ClosestDistance = Distance;
				ClosestActor = Actor;
			}
		}
	} // for문을 돌아 가장 가까운 Actor를 ClosestActor로 업데이트.
	UBTFunctionLibrary::SetBlackboardValueAsObject(this, TargetToFollowSelector, ClosestActor);
	UBTFunctionLibrary::SetBlackboardValueAsFloat(this, DistanceToTargetSelector, ClosestDistance);
}
