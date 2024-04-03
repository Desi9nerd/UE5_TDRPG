#include "Character/TDBaseCharacter.h"
#include "AbilitySystemComponent.h"
#include "GAS/TDAbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameplayTags/TDGameplayTags.h"
#include "TDRPG/TDRPG.h" // ECC_Projectile

ATDBaseCharacter::ATDBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	//** Mesh
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	// 카메라랑 Capsule, Mesh 충돌 무시
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	// Projectile채널(=ECC_Projectile)을 Overlap 처리
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true); // Mesh에 Overlap 이벤트 발생 true로 설정

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

UAbilitySystemComponent* ATDBaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAnimMontage* ATDBaseCharacter::GetHitReactMontage_Implementation()
{
	return HitReactMontage;
}

void ATDBaseCharacter::Die()
{
	Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));

	MulticastHandleDeath(); // 캐릭터 사망 처리. Server & Client
}

void ATDBaseCharacter::MulticastHandleDeath_Implementation() // 캐릭터 사망 처리
{
	// 무기 바닥에 떨어뜨리기
	Weapon->SetSimulatePhysics(true);
	Weapon->SetEnableGravity(true);
	Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	// Ragdoll 적용을 위해 캐릭터 매쉬/캡슐 설정하기 
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision); // Capsule 충돌X

	bDead = true;
}

void ATDBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATDBaseCharacter::InitAbilityActorInfo()
{
}

void ATDBaseCharacter::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const // GE 적용하기
{
	check(GetAbilitySystemComponent()); // ASC 유무 검사
	check(GameplayEffectClass); 

	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this); // SourceObject(=TDBaseCharacter)를 ContextHandle에 추가. ICombat.h의 GetPlayerLevel()를 사용할때 필요하기에 SourceObject을 추가

	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass, Level, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
}

void ATDBaseCharacter::InitializeDefaultAttributes() const // Attributes 초기값 설정
{
	// Stat/Secondary/VitalAttributes로 사용할 GE_Stat/Secondary/VitalAttributes 적용
	ApplyEffectToSelf(DefaultStatAttributes, 1.f);	
	ApplyEffectToSelf(DefaultSecondaryAttributes, 1.f); 
	ApplyEffectToSelf(DefaultVitalAttributes, 1.f);
}

void ATDBaseCharacter::AddCharacterAbilities() // 서버에서만 실행.
{
	UTDAbilitySystemComponent* TDASC = CastChecked<UTDAbilitySystemComponent>(AbilitySystemComponent);
	if (false == HasAuthority()) return; // 서버가 아니면 return

	TDASC->AddCharacterAbilities(StartupAbilities);
}

FVector ATDBaseCharacter::GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag) // 소켓 위치를 리턴
{
	const FTDGameplayTags& GameplayTags = FTDGameplayTags::GetTDGameplayTags();
	if (MontageTag.MatchesTagExact(GameplayTags.Montage_Attack_Weapon) && IsValid(Weapon))
	{
		return Weapon->GetSocketLocation(WeaponTipSocketName);
	}
	if (MontageTag.MatchesTagExact(GameplayTags.Montage_Attack_Head))
	{
		return GetMesh()->GetSocketLocation(HeadSocketName);
	}
	if (MontageTag.MatchesTagExact(GameplayTags.Montage_Attack_LeftHand))
	{
		return GetMesh()->GetSocketLocation(LeftHandSocketName);
	}
	if (MontageTag.MatchesTagExact(GameplayTags.Montage_Attack_RightHand))
	{
		return GetMesh()->GetSocketLocation(RightHandSocketName);
	}

	return FVector();
}

FVector ATDBaseCharacter::GetCombatSocketLocationCPP(const FGameplayTag& MontageTag) // 소켓 위치를 리턴
{
	const FTDGameplayTags& GameplayTags = FTDGameplayTags::GetTDGameplayTags();
	if (MontageTag.MatchesTagExact(GameplayTags.Montage_Attack_Weapon) && IsValid(Weapon))
	{
		return Weapon->GetSocketLocation(WeaponTipSocketName);
	}
	if (MontageTag.MatchesTagExact(GameplayTags.Montage_Attack_Head))
	{
		return GetMesh()->GetSocketLocation(HeadSocketName);
	}
	if (MontageTag.MatchesTagExact(GameplayTags.Montage_Attack_LeftHand))
	{
		return GetMesh()->GetSocketLocation(LeftHandSocketName);
	}
	if (MontageTag.MatchesTagExact(GameplayTags.Montage_Attack_RightHand))
	{
		return GetMesh()->GetSocketLocation(RightHandSocketName);
	}

	return FVector();
}

bool ATDBaseCharacter::IsDead_Implementation() const
{
	return bDead;
}

AActor* ATDBaseCharacter::GetAvatar_Implementation()
{
	return this;
}

TArray<FTaggedMontage> ATDBaseCharacter::GetAttackMontages_Implementation()
{
	return AttackMontages;
}

TArray<FTaggedMontage> ATDBaseCharacter::GetAttackMontagesCPP()
{
	return AttackMontages;
}

AActor* ATDBaseCharacter::GetCombatTargetCPP() const
{
	return IICombat::GetCombatTarget();
}

UAnimMontage* ATDBaseCharacter::GetHitReactMontageFromCharacter()
{
	return HitReactMontage;
}
