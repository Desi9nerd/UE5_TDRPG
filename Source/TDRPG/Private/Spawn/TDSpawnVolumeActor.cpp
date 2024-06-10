#include "Spawn/TDSpawnVolumeActor.h"
#include "Components/BoxComponent.h"
#include "Interface/IPlayer.h"
#include "Spawn/TDTargetPoint_Spawn.h"

ATDSpawnVolumeActor::ATDSpawnVolumeActor()
{
	PrimaryActorTick.bCanEverTick = false;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	SetRootComponent(Box);
	Box->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Box->SetCollisionObjectType(ECC_WorldStatic);
	Box->SetCollisionResponseToAllChannels(ECR_Ignore);
	Box->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void ATDSpawnVolumeActor::LoadActor()
{
	if (bReached) // 저장된 게임에서 이전에 충돌 트리거 된적이 있다면 true이므로 또 몬스터가 스폰되지 않도록 한다.
	{
		Destroy(); // 제거.
	}
}

void ATDSpawnVolumeActor::BeginPlay()
{
	Super::BeginPlay();

	Box->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBoxOverlap);
}

void ATDSpawnVolumeActor::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IIPlayer* PlayerInterface = Cast<IIPlayer>(OtherActor);
	if (nullptr == PlayerInterface) return; // 충돌 대상이 플레이어가 아니라면 return.

	bReached = true; // 박스에 닿으면 true 설정. 게임 저장 후 다음에 실행할 때 사용하지 않기 위해 bool 변수 사용.
	for (ATDTargetPoint_Spawn* Point : SpawnPoints)
	{
		if (IsValid(Point))
		{
			Point->SpawnEnemy(); // TargetPoint에 몬스터 스폰
		}
	}

	Box->SetCollisionEnabled(ECollisionEnabled::NoCollision); // 스폰 되었으니 또 스폰되지 않도록 충돌 꺼줌.
}
