#include "Inventory/Item/Actors/TDItemActor.h"
#include "Net/UnrealNetwork.h"
#include "Engine/ActorChannel.h"
#include "Inventory/Item/TDItemInstance.h"
#include "Kismet/KismetSystemLibrary.h"

ATDItemActor::ATDItemActor()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}

void ATDItemActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATDItemActor, TDItemInstance);
}

bool ATDItemActor::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool bRenewed = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	
	// Channel�� ���� TDItemInstance�� ����. ��Ʈ��ũ�� ���� �ش� TDItemInstance�� ���°� Ŭ���̾�Ʈ�� ����ȭ�ǵ��� ��.
	bRenewed |= Channel->ReplicateSubobject(TDItemInstance, *Bunch, *RepFlags);

	return bRenewed;
}

void ATDItemActor::InitItemActor(UTDItemInstance* InTDItemInstance)
{
	TDItemInstance = InTDItemInstance;
	
}

void ATDItemActor::OnEquipped()
{
}

void ATDItemActor::OnUnequipped()
{
}

void ATDItemActor::OnDropped()
{
	GetRootComponent()->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);

	if (AActor* ActorOwner = GetOwner())
	{
		const FVector CurrentLocation = GetActorLocation();
		const FVector ForwardDirection = ActorOwner->GetActorForwardVector();
		
		const FVector TraceStartLocation = CurrentLocation - ForwardDirection * 10.f;
		const FVector TraceEndLocation = TraceStartLocation - FVector(0.f, 0.f, 1.f) * 500.f;

		TArray<AActor*> ActorsToIgnore = { GetOwner() };
		FHitResult HitResult;
		bool bHit = UKismetSystemLibrary::LineTraceSingleByProfile(this, TraceStartLocation, TraceEndLocation, TEXT("WorldStatic"), true, ActorsToIgnore, EDrawDebugTrace::None, HitResult, true);
		if (bHit)
		{
			if (HitResult.bBlockingHit)
			{
				SetActorLocation(HitResult.Location);
				return;
			}
		}

		SetActorLocation(TraceEndLocation);
	}
}

void ATDItemActor::OnRep_TDItemInstance(UTDItemInstance* OldTDItemInstance)
{

}
