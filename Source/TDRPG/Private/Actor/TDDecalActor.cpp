#include "Actor/TDDecalActor.h"
#include "Components/DecalComponent.h"

ATDDecalActor::ATDDecalActor()
{
	PrimaryActorTick.bCanEverTick = true;

	DecalCircle = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalCircle"));
	DecalCircle->SetupAttachment(GetRootComponent());
}

void ATDDecalActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATDDecalActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 데칼 돌리기
	DecalCircle->AddLocalRotation(FRotator(0.f, 0.f, 2.f));
}
