#include "Character/TDBaseCharacter.h"
#include "AbilitySystemComponent.h"
#include "MotionWarpingComponent.h"
#include "GAS/TDAbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameplayTags/TDGameplayTags.h"
#include "Kismet/GameplayStatics.h"
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

	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>("MotionWarpingComponent");

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

UAbilitySystemComponent* ATDBaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ATDBaseCharacter::UpdateFacingTargetCPP(const FVector& FacingTarget)
{
	// 몽타주에서 MotionWarping 애님스테이트를 지정하고 Warp Target Name을 똑같이 적는다
	MotionWarpingComponent->AddOrUpdateWarpTargetFromLocation(WarpTargetName, FacingTarget);
}

UAnimMontage* ATDBaseCharacter::GetHitReactMontage_Implementation()
{
	return HitReactMontage;
}

UAnimMontage* ATDBaseCharacter::GetHitReactMontageCPP()
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
	// 사망 사운드 재생
	UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation(), GetActorRotation());

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
	TDASC->AddCharacterPassiveAbilities(StartupPassiveAbilities);
}

FVector ATDBaseCharacter::GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag) // 소켓 위치를 리턴
{
	const FTDGameplayTags& GameplayTags = FTDGameplayTags::GetTDGameplayTags();
	if (MontageTag.MatchesTagExact(GameplayTags.Socket_Weapon) && IsValid(Weapon))
	{
		return Weapon->GetSocketLocation(WeaponTipSocketName);
	}
	if (MontageTag.MatchesTagExact(GameplayTags.Socket_Head))
	{
		return GetMesh()->GetSocketLocation(HeadSocketName);
	}
	if (MontageTag.MatchesTagExact(GameplayTags.Socket_LeftHand))
	{
		return GetMesh()->GetSocketLocation(LeftHandSocketName);
	}
	if (MontageTag.MatchesTagExact(GameplayTags.Socket_RightHand))
	{
		return GetMesh()->GetSocketLocation(RightHandSocketName);
	}
	if (MontageTag.MatchesTagExact(GameplayTags.Socket_Trail))
	{
		return GetMesh()->GetSocketLocation(TrailSocketName);
	}

	return FVector();
}

FVector ATDBaseCharacter::GetCombatSocketLocationCPP(const FGameplayTag& MontageTag)
{
	const FTDGameplayTags& GameplayTags = FTDGameplayTags::GetTDGameplayTags();
	if (MontageTag.MatchesTagExact(GameplayTags.Socket_Weapon) && IsValid(Weapon))
	{
		return Weapon->GetSocketLocation(WeaponTipSocketName);
	}
	if (MontageTag.MatchesTagExact(GameplayTags.Socket_Head))
	{
		return GetMesh()->GetSocketLocation(HeadSocketName);
	}
	if (MontageTag.MatchesTagExact(GameplayTags.Socket_LeftHand))
	{
		return GetMesh()->GetSocketLocation(LeftHandSocketName);
	}
	if (MontageTag.MatchesTagExact(GameplayTags.Socket_RightHand))
	{
		return GetMesh()->GetSocketLocation(RightHandSocketName);
	}
	if (MontageTag.MatchesTagExact(GameplayTags.Socket_Trail))
	{
		return GetMesh()->GetSocketLocation(TrailSocketName);
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
	return ATKMontages;
}

TArray<FTaggedMontage> ATDBaseCharacter::GetAttackMontagesCPP()
{
	return ATKMontages;
}

UNiagaraSystem* ATDBaseCharacter::GetBloodEffect_Implementation()
{
	return BloodEffect;
}

UNiagaraSystem* ATDBaseCharacter::GetBloodEffectCPP()
{
	return BloodEffect;
}

FTaggedMontage ATDBaseCharacter::GetTaggedMontageByTag_Implementation(const FGameplayTag& MontageTag)
{
	for (FTaggedMontage TaggedMontage : ATKMontages)
	{
		if (TaggedMontage.MontageTag == MontageTag)
		{
			return TaggedMontage;
		}
	}
	return FTaggedMontage();
}

FTaggedMontage ATDBaseCharacter::GetTaggedMontageByTagCPP(const FGameplayTag& MontageTag)
{
	for (FTaggedMontage TaggedMontage : ATKMontages)
	{
		if (TaggedMontage.MontageTag == MontageTag)
		{
			return TaggedMontage;
		}
	}
	return FTaggedMontage();
}

int32 ATDBaseCharacter::GetMinionCount_Implementation()
{
	return MinionCnt;
}

int32 ATDBaseCharacter::GetMinionCountCPP()
{
	return MinionCnt;
}

ECharacterClass ATDBaseCharacter::GetCharacterClass_Implementation()
{
	return CharacterClass;
}

ECharacterClass ATDBaseCharacter::GetCharacterClassCPP()
{
	return CharacterClass;
}

void ATDBaseCharacter::IncremenetMinionCount_Implementation(int32 Amount)
{
	MinionCnt += Amount;
}

void ATDBaseCharacter::IncremenetMinionCntCPP(int32 Amount)
{
	MinionCnt += Amount;
}
