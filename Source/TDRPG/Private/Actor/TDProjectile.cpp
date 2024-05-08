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
	bReplicates = true; // ���Ͱ� replicated�� �� �ֵ��� true ����

	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	SetRootComponent(Sphere);
	Sphere->SetCollisionObjectType(ECC_Projectile); // Custom���� ������ ECollisionChannel
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

	LoopingSoundComponent = UGameplayStatics::SpawnSoundAttached(LoopingSound, GetRootComponent()); // LoopingSound ���
}

void ATDProjectile::Destroyed()
{
	if (false == bHit && false == HasAuthority()) // �ǰ�X && Client
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());
		if (IsValid(LoopingSoundComponent))
		{
			LoopingSoundComponent->Stop(); // LoopingSound ��� ���߱�
		}
		bHit = true;
	}

	Super::Destroyed();
}

void ATDProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// ���1: GameplayEffectSpecHandle
	if (false == DamageEffectSpecHandle.Data.IsValid()) return; // ����ó��
	if (DamageEffectSpecHandle.Data.Get()->GetContext().GetEffectCauser() == OtherActor) return; // ���͸� ���� �ڱ��ڽ��� Overlap���� �ʰ� ����.
	if (UTDAbilitySystemBPLibrary::IsSameTeam(DamageEffectSpecHandle.Data.Get()->GetContext().GetEffectCauser(), OtherActor))
	{
		return; // ����ó��. ���� ���̸� �ǰݵ��� �ʵ��� �ٷ� ����.
	}

	// ���2: DamageEffectParams
	AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();
	if (IsValid(SourceAvatarActor)) return;
	if (SourceAvatarActor == OtherActor) return; // �ڱ��ڽ� ����ó��.
	if (UTDAbilitySystemBPLibrary::IsSameTeam(SourceAvatarActor, OtherActor)) return; // ���� ���̸� �ǰݵ��� �ʵ��� �ٷ� ����.


	if (false == bHit) // �ǰ� ���� ���� �ÿ� ����. bHit�� �Ʒ����� false�� ����ȴ�.
	{
		//UE_LOG(LogTemp, Warning, TEXT("%s spawned"), *GetName()); // �����Ǵ� Projectile ���� Ȯ�ο�

		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());
		if (IsValid(LoopingSoundComponent))
		{
			LoopingSoundComponent->Stop(); // LoopingSound ��� ���߱�
		}
		bHit = true;
	}


	if (HasAuthority()) // Server
	{
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor);
		if (IsValid(TargetASC))
		{
			// ���1: GameplayEffectSpecHandle
			TargetASC->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data.Get());

			// ���2: DamageEffectParams
			DamageEffectParams.TargetAbilitySystemComponent = TargetASC;
			UTDAbilitySystemBPLibrary::ApplyDamageEffect(DamageEffectParams);
		}

		Destroy();
	}
	else // Client
	{
		bHit = true; // �ǰ�O
	}
}
