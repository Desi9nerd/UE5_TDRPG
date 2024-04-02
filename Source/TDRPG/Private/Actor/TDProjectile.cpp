#include "Actor/TDProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TDRPG/TDRPG.h" // ECC_Projectile
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GAS/TDAbilitySystemBPLibrary.h"

ATDProjectile::ATDProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true; // 액터가 replicated될 수 있도록 true 설정

	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	SetRootComponent(Sphere);
	Sphere->SetCollisionObjectType(ECC_Projectile); // Custom으로 지정한 ECollisionChannel
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->InitialSpeed = 550.f;
	ProjectileMovement->MaxSpeed = 550.f;
	ProjectileMovement->ProjectileGravityScale = 0.f;
}

void ATDProjectile::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(LifeSpan);

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnSphereOverlap);

	LoopingSoundComponent = UGameplayStatics::SpawnSoundAttached(LoopingSound, GetRootComponent()); // LoopingSound 재생
}

void ATDProjectile::Destroyed()
{
	if (false == bHit && false == HasAuthority()) // 피격X && Client
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());
		if (IsValid(LoopingSoundComponent))
		{
			LoopingSoundComponent->Stop(); // LoopingSound 재생 멈추기
		}
	}

	Super::Destroyed();
}

void ATDProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (false == DamageEffectSpecHandle.Data.IsValid()) return; // 예외처리
	if (DamageEffectSpecHandle.Data.Get()->GetContext().GetEffectCauser() == OtherActor) return; // 액터를 던진 자기자신은 Overlap되지 않게 리턴.
	
	if (UTDAbilitySystemBPLibrary::IsSameTeam(DamageEffectSpecHandle.Data.Get()->GetContext().GetEffectCauser(), OtherActor))
	{
		return; // 예외처리. 같은 팀이면 피격되지 않도록 바로 리턴.
	}


	if (false == bHit) // 피격 최초 성공 시에 적용. bHit은 아래에서 false로 변경된다.
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());
		if (IsValid(LoopingSoundComponent))
		{
			LoopingSoundComponent->Stop(); // LoopingSound 재생 멈추기
		}
	}

	if (HasAuthority()) // Server
	{
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor);
		if (IsValid(TargetASC))
		{
			TargetASC->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data.Get());
		}

		Destroy();
	}
	else // Client
	{
		bHit = true; // 피격O
	}
}
