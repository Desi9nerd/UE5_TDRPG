#include "Actor/TDItemActor.h"
#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"

ATDItemActor::ATDItemActor()
{
	PrimaryActorTick.bCanEverTick = false; // false

	bReplicates = true;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(GetRootComponent());
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SphereCollision = CreateDefaultSubobject<USphereComponent>("SphereCollision");
	SphereCollision->SetupAttachment(Mesh);
	SphereCollision->SetSphereRadius(100.f);
}

void ATDItemActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(ATDItemActor, ItemQuantity, COND_None, REPNOTIFY_Always);
	//DOREPLIFETIME_CONDITION_NOTIFY(ATDItemActor, bDestroyItem, COND_None, REPNOTIFY_Always); // 삭제예정: 옵션1
}

void ATDItemActor::DestroyItem()
{
	Destroy();
}

void ATDItemActor::BeginPlay()
{
	Super::BeginPlay();

	Mesh->SetSimulatePhysics(true);
}

void ATDItemActor::OnSphereComponentOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Item Overlapped! "));
	}
}

//void ATDItemActor::OnDestroyRootItem(bool InbDestroyItem)
//{
//	bDestroyItem = InbDestroyItem;
//}
//
//void ATDItemActor::OnRep_DestroyItem()
//{
//	FTimerHandle TestTimerHanlde;
//	if (bDestroyItem)
//	{
//		Destroy(); // 액터 소멸.
//	}
//}

// 오대성님, 
// 얕은 복사
// Destroy