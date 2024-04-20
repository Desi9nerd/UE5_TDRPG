#include "Inventory/Item/Actors/TDItemActor.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/ActorChannel.h"
#include "GameplayTags/TDGameplayTags.h"
#include "Inventory/TDInventoryComponent.h"
#include "Inventory/Item/TDItemInstance.h"
#include "Kismet/KismetSystemLibrary.h"

ATDItemActor::ATDItemActor()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicateMovement(true); // 위치, 회전 등 움직임이 replicated 되도록 true 설정.

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
	ItemState = EItemState::EIS_Equipped;

	SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SphereComponent->SetGenerateOverlapEvents(false);
}

void ATDItemActor::OnUnequipped()
{
	ItemState = EItemState::EIS_None;

	SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SphereComponent->SetGenerateOverlapEvents(false);
}

void ATDItemActor::OnDropped()
{
	ItemState = EItemState::EIS_Dropped;

	GetRootComponent()->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);

	if (AActor* ActorOwner = GetOwner())
	{
		const FVector CurrentLocation = GetActorLocation();
		const FVector ForwardDirection = ActorOwner->GetActorForwardVector();
		
		const FVector TraceStartLocation = CurrentLocation - ForwardDirection * 10.f;
		const FVector TraceEndLocation = TraceStartLocation - FVector(0.f, 0.f, 1.f) * 500.f;

		// 디버깅용 EDrawDebugTrace타입 설정: ShowDebugInventory1일 때는 ForDurtaion, 0일때는 None.
		static const auto CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("ShowDebugInventory"));
		const bool bShowInventory = CVar->GetInt() > 0;
		EDrawDebugTrace::Type DebugDrawType = bShowInventory ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;

		TArray<AActor*> ActorsToIgnore = { GetOwner() };
		FHitResult HitResult;
		FVector TargetDroppingLocation = TraceEndLocation;
		bool bHit = UKismetSystemLibrary::LineTraceSingleByProfile(this, TraceStartLocation, TraceEndLocation, TEXT("WorldStatic"), true, ActorsToIgnore, DebugDrawType, HitResult, true);
		if (bHit)
		{
			if (HitResult.bBlockingHit)
			{
				TargetDroppingLocation = HitResult.Location;
			}
		}

		SetActorLocation(TargetDroppingLocation);

		SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		SphereComponent->SetGenerateOverlapEvents(true);
	}
}

void ATDItemActor::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority()) // Server
	{
		if (false == IsValid(TDItemInstance))
		{
			TDItemInstance = NewObject<UTDItemInstance>();

			// 게임 시작 시 Collsion 켜주기.
			SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			SphereComponent->SetGenerateOverlapEvents(true);
		}
	}
}

void ATDItemActor::OnRep_TDItemInstance()
{
	if (IsValid(TDItemInstance))
	{
		// TODO
	}
}

void ATDItemActor::OnRep_ItemState()
{
	switch (ItemState)
	{
	case EItemState::EIS_Stored:
		SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SphereComponent->SetGenerateOverlapEvents(false);
		break;
	case EItemState::EIS_Equipped:
		SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SphereComponent->SetGenerateOverlapEvents(false);
		break;
	case EItemState::EIS_Dropped:
		SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		SphereComponent->SetGenerateOverlapEvents(true);
		break;
	default:
		SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		SphereComponent->SetGenerateOverlapEvents(true);
		break;
	}
}

void ATDItemActor::OnSphereComponentOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (HasAuthority())
	{
		FGameplayEventData EventPayload;
		EventPayload.Instigator = this;
		EventPayload.OptionalObject = TDItemInstance;
		EventPayload.EventTag = FTDGameplayTags::GetTDGameplayTags().Item_Equip;

		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OtherActor, EventPayload.EventTag, EventPayload);
	}
}
