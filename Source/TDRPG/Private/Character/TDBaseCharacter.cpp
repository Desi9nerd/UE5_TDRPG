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

	// ī�޶�� Capsule, Mesh �浹 ����
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	// Projectileä��(=ECC_Projectile)�� Overlap ó��
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true); // Mesh�� Overlap �̺�Ʈ �߻� true�� ����

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
	// ��Ÿ�ֿ��� MotionWarping �ִԽ�����Ʈ�� �����ϰ� Warp Target Name�� �Ȱ��� ���´�
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

	MulticastHandleDeath(); // ĳ���� ��� ó��. Server & Client
}

void ATDBaseCharacter::MulticastHandleDeath_Implementation() // ĳ���� ��� ó��
{
	// ��� ���� ���
	UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation(), GetActorRotation());

	// ���� �ٴڿ� ����߸���
	Weapon->SetSimulatePhysics(true);
	Weapon->SetEnableGravity(true);
	Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	// Ragdoll ������ ���� ĳ���� �Ž�/ĸ�� �����ϱ� 
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision); // Capsule �浹X

	bDead = true;
}

void ATDBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATDBaseCharacter::InitAbilityActorInfo()
{
}

void ATDBaseCharacter::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const // GE �����ϱ�
{
	check(GetAbilitySystemComponent()); // ASC ���� �˻�
	check(GameplayEffectClass); 

	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this); // SourceObject(=TDBaseCharacter)�� ContextHandle�� �߰�. ICombat.h�� GetPlayerLevel()�� ����Ҷ� �ʿ��ϱ⿡ SourceObject�� �߰�

	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass, Level, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
}

void ATDBaseCharacter::InitializeDefaultAttributes() const // Attributes �ʱⰪ ����
{
	// Stat/Secondary/VitalAttributes�� ����� GE_Stat/Secondary/VitalAttributes ����
	ApplyEffectToSelf(DefaultStatAttributes, 1.f);	
	ApplyEffectToSelf(DefaultSecondaryAttributes, 1.f); 
	ApplyEffectToSelf(DefaultVitalAttributes, 1.f);
}

void ATDBaseCharacter::AddCharacterAbilities() // ���������� ����.
{
	UTDAbilitySystemComponent* TDASC = CastChecked<UTDAbilitySystemComponent>(AbilitySystemComponent);
	if (false == HasAuthority()) return; // ������ �ƴϸ� return

	TDASC->AddCharacterAbilities(StartupAbilities);
	TDASC->AddCharacterPassiveAbilities(StartupPassiveAbilities);
}

FVector ATDBaseCharacter::GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag) // ���� ��ġ�� ����
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
