#include "AI/BTService/BTService_SearchPlayer.h"
#include "AIController.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

void UBTService_SearchPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// BTService_BlueprintBase�� AIOwner, ActorOwner ������ ������.
	/** TObjectPtr<AAIController> AIOwner; -  Cached AIController owner of BehaviorTreeComponent. */
	/** TObjectPtr<AActor> ActorOwner - Cached actor owner of BehaviorTreeComponent. */

	//GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, *AIOwner->GetName());
	//GEngine->AddOnScreenDebugMessage(2, 1.f, FColor::Green, *ActorOwner->GetName());

	APawn* OwningPawn = AIOwner->GetPawn();

	// BP_Player�� BP_Enemy���� �������� Actor Tag ���.
	// Player ���忡���� TargetTag�� "Enemy", Enemy���忡���� TargetTag�� "Player".
	const FName TargetTag = OwningPawn->ActorHasTag(FName("Player")) ? FName("Enemy") : FName("Player");

	TArray<AActor*> ActorsWithTag;
	UGameplayStatics::GetAllActorsWithTag(OwningPawn, TargetTag, ActorsWithTag); // TargetTag�� ��ġ�ϴ� Actor Tag�� ���� ��� Pawn���� ActorsWithTag ���͹迭�� ��´�.

	float ClosestDistance = TNumericLimits<float>::Max(); // �ִ밪�� �⺻������ ����.
	AActor* ClosestActor = nullptr; // ���� ����� Actor
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
	} // for���� ���� ���� ����� Actor�� ClosestActor�� ������Ʈ.
	UBTFunctionLibrary::SetBlackboardValueAsObject(this, TargetToFollowSelector, ClosestActor);
	UBTFunctionLibrary::SetBlackboardValueAsFloat(this, DistanceToTargetSelector, ClosestDistance);
}
