﻿#include "SaveGame/TDPlayerStart.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "GameMode/TDGameModeBase_Single.h"
#include "Interface/IPlayer.h"
#include "Kismet/GameplayStatics.h"

ATDPlayerStart::ATDPlayerStart(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;

	CheckpointMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CheckpointMesh"));
	CheckpointMesh->SetupAttachment(GetRootComponent());
	CheckpointMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CheckpointMesh->SetCollisionResponseToAllChannels(ECR_Block);

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(CheckpointMesh);
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void ATDPlayerStart::LoadActor()
{
	if (bReached)
	{
		HandleGlowEffects();
	}
}

void ATDPlayerStart::BeginPlay()
{
	Super::BeginPlay();

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnSphereOverlap);
	
}

void ATDPlayerStart::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IIPlayer* PlayerInterface = Cast<IIPlayer>(OtherActor);
	if (PlayerInterface) // 플레이어라면
	{
		bReached = true;

		if (ATDGameModeBase_Single* TDGameMode = Cast<ATDGameModeBase_Single>(UGameplayStatics::GetGameMode(this)))
		{
			const UWorld* World = GetWorld();
			FString MapName = World->GetMapName();
			MapName.RemoveFromStart(World->StreamingLevelsPrefix);

			TDGameMode->SaveWorldState(GetWorld()); // SaveMap에 월드 정보를 저장.
		}

		PlayerInterface->SaveProgress(PlayerStartTag); // PlayerStartTag를 넘겨 저장.
		HandleGlowEffects(); // Glow 효과.
	}
}

void ATDPlayerStart::HandleGlowEffects()
{
	Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision); // 충돌X

	UMaterialInstanceDynamic* DynamicMaterialInstace = UMaterialInstanceDynamic::Create(CheckpointMesh->GetMaterial(0), this);
	CheckpointMesh->SetMaterial(0, DynamicMaterialInstace);
	CheckpointReached(DynamicMaterialInstace);
}
