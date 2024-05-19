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
	SetActorTickInterval(0.1f);

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SetRootComponent(Sphere);
	Sphere->SetCollisionObjectType(ECC_Projectile); // Custom으로 지정한 ECollisionChannel
	Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 550.f;
	ProjectileMovement->MaxSpeed = 550.f;
	ProjectileMovement->ProjectileGravityScale = 0.f;
}

void ATDProjectile::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(LifeSpan);
	SetReplicateMovement(true);

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnSphereOverlap);

	LoopingSoundComponent = UGameplayStatics::SpawnSoundAttached(LoopingSound, GetRootComponent()); // LoopingSound 재생

	FTimerHandle DelayTimer;

	FTimerDelegate TimerDelegate = FTimerDelegate::CreateLambda([this]() {
		Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		});

	GetWorld()->GetTimerManager().SetTimer(DelayTimer, TimerDelegate, 0.1f, false); // 0.1초 지난 후 Sphere Collision을 QueryOnly로 변경.
}

void ATDProjectile::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	LocationLastFrame = LocationThisFrame;
	LocationThisFrame = GetActorLocation();

	if ((LocationThisFrame - LocationLastFrame).Length() < 10.f )
	{
		UGameplayStatics::SpawnSoundAtLocation(this, ImpactSound, LocationThisFrame, FRotator::ZeroRotator);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, LocationThisFrame);
		Destroy();
	}
}

void ATDProjectile::Destroyed()
{
	if (false == bHit && false == HasAuthority()) // 피격X && Client
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
		if (IsValid(ImpactEffect))
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());
		}
		else
		{
			UGameplayStatics::SpawnEmitterAtLocation(this, ImpactEffect_ParticleSystem, GetActorLocation());
		}

		if (IsValid(LoopingSoundComponent))
		{
			LoopingSoundComponent->Stop(); // LoopingSound 재생 멈추기
			LoopingSoundComponent->DestroyComponent();
		}
		bHit = true;
	}

	Super::Destroyed();
}

void ATDProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 방법1: GameplayEffectSpecHandle
	//if (false == DamageEffectSpecHandle.Data.IsValid()) return; // 예외처리
	//if (DamageEffectSpecHandle.Data.Get()->GetContext().GetEffectCauser() == OtherActor) return; // 액터를 던진 자기자신은 Overlap되지 않게 리턴.
	//if (UTDAbilitySystemBPLibrary::IsSameTeam(DamageEffectSpecHandle.Data.Get()->GetContext().GetEffectCauser(), OtherActor))
	//{
	//	return; // 예외처리. 같은 팀이면 피격되지 않도록 바로 리턴.
	//}

	// 방법2: DamageEffectParams
	if (DamageEffectParams.SourceAbilitySystemComponent == nullptr) return;
	AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();
	if (false == IsValid(SourceAvatarActor)) return;
	if (SourceAvatarActor == OtherActor) return; // 액터를 던진 자기자신은 Overlap되지 않게 예외처리 리턴.
	if (UTDAbilitySystemBPLibrary::IsSameTeam(SourceAvatarActor, OtherActor)) return; // 같은 팀이면 피격되지 않도록 바로 리턴.


	if (false == bHit) // 피격 최초 성공 시에 적용. bHit은 아래에서 false로 변경된다.
	{
		//UE_LOG(LogTemp, Warning, TEXT("%s spawned"), *GetName()); // 스폰되는 Projectile 개수 확인용

		//* Hit Sound
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
		//* Hit Effect
		if (IsValid(ImpactEffect)) // Niagara
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());
		}
		else // ParticleSystem
		{
			UGameplayStatics::SpawnEmitterAtLocation(this, ImpactEffect_ParticleSystem, GetActorLocation());
		}
		//* Looping  Sound Turning Off
		if (IsValid(LoopingSoundComponent))
		{
			LoopingSoundComponent->Stop(); // LoopingSound 재생 멈추기
			LoopingSoundComponent->DestroyComponent();
		}
		bHit = true;
	}


	if (HasAuthority()) // Server
	{
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor);
		if (IsValid(TargetASC))
		{
			// 방법1: GameplayEffectSpecHandle
			//TargetASC->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data.Get());

			//************************************************************************************
			// 방법2: DamageEffectParams

			// **** DamageEffectParams을 업데이트. 업데이트 된 후 GameplayAbility(GA)에서 가져다가 사용. ex. TDGA_DamageProjectile
			// Ragdoll
			const FVector RagdollImpulse = GetActorForwardVector() * DamageEffectParams.RagdollImpulseMagnitude;
			DamageEffectParams.RagdollImpulse = RagdollImpulse;
			// Knockback
			const bool bKnockback = FMath::RandRange(1, 100) < DamageEffectParams.KnockbackChance;
			if (bKnockback)
			{
				FRotator Rotation = GetActorRotation();
				Rotation.Pitch = 45.f;

				const FVector KnockbackDirection = Rotation.Vector(); // 이 액터(=TDProjectile)의 방향
				const FVector KnockbackImpulse = KnockbackDirection * DamageEffectParams.KnockbackImpulseMagnitude;
				DamageEffectParams.KnockbackImpulse = KnockbackImpulse;
			}
			// Target AbilitySystemComponent
			DamageEffectParams.TargetAbilitySystemComponent = TargetASC;
			// **** DamageEffectParams을 업데이트 완료.

			UTDAbilitySystemBPLibrary::ApplyDamageEffect(DamageEffectParams); // BP나 전역적으로 사용할 수 있도록 UTDAbilitySystemBPLibrary에도 업데이트

			//************************************************************************************
		}

		Destroy();
	}
	else // Client
	{
		bHit = true; // 피격O
	}
}
