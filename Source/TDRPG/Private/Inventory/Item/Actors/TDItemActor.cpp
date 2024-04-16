#include "Inventory/Item/Actors/TDItemActor.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/ActorChannel.h"
#include "Inventory/Item/TDItemInstance.h"
#include "Kismet/KismetSystemLibrary.h"

ATDItemActor::ATDItemActor()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(RootComponent);
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnSphereComponentOverlap);
}

void ATDItemActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATDItemActor, TDItemInstance);
	DOREPLIFETIME(ATDItemActor, ItemState);
}

bool ATDItemActor::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool bRenewed = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	
	// Channel을 통해 TDItemInstance를 복제. 네트워크를 통해 해당 TDItemInstance의 상태가 클라이언트에 동기화되도록 함.
	bRenewed |= Channel->ReplicateSubobject(TDItemInstance, *Bunch, *RepFlags);

	return bRenewed;
}

void ATDItemActor::InitItemActor(UTDItemInstance* InTDItemInstance)
{
	TDItemInstance = InTDItemInstance;
	
}

void ATDItemActor::OnEquipped()
{
	ItemState = EItemState::Equipped;

	SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ATDItemActor::OnUnequipped()
{
	ItemState = EItemState::None;

	SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ATDItemActor::OnDropped()
{
	ItemState = EItemState::Dropped;

	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);


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

void ATDItemActor::OnRep_ItemState()
{
}

void ATDItemActor::OnSphereComponentOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (HasAuthority())
	{
		FGameplayEventData EventPayload;
		EventPayload.Instigator = this;
		EventPayload.OptionalObject = TDItemInstance;

		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OtherActor, OverlapEventTag, EventPayload);
	}
}
