#include "Actor/TDItemActor.h"
#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"

ATDItemActor::ATDItemActor()
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(GetRootComponent());
	SphereCollision = CreateDefaultSubobject<USphereComponent>("SphereCollision");
	SphereCollision->SetupAttachment(Mesh);
}

void ATDItemActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(ATDItemActor, bDestroyItem, COND_None, REPNOTIFY_Always);
}

void ATDItemActor::BeginPlay()
{
	Super::BeginPlay();

	Mesh->SetSimulatePhysics(true);
}

void ATDItemActor::SetItemQuantity(int32 InItemQuantity)
{
	ItemQuantity = InItemQuantity;
}

void ATDItemActor::OnDestroyRootItem(bool InbDestroyItem)
{
	bDestroyItem = InbDestroyItem;
}

void ATDItemActor::OnRep_DestroyItem()
{
	if (bDestroyItem)
	{
		Destroy(); // 액터 소멸.
	}
}
