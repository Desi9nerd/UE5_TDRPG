#include "Character/TDBaseCharacter.h"
#include "Net/UnrealNetwork.h"
#include "AbilitySystemComponent.h"
#include "MotionWarpingComponent.h"
#include "Component/TDDebuffComponent.h"
#include "GAS/TDAbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameplayTags/TDGameplayTags.h"
#include "Kismet/GameplayStatics.h"
#include "TDRPG/TDRPG.h" // ECC_Projectile

ATDBaseCharacter::ATDBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

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

	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarpingComponent"));
	DotDamageDebuffComponent = CreateDefaultSubobject<UTDDebuffComponent>(TEXT("DotDamageDebuffComponent"));
	DotDamageDebuffComponent->SetupAttachment(GetRootComponent());
	DotDamageDebuffComponent->DebuffTag = FTDGameplayTags::GetTDGameplayTags().Debuff_DotDamage; // DebuffTag�� �⺻��. ���� �׽�Ʈ�� ���� DotDamage�� ����.
	StunDebuffComponent = CreateDefaultSubobject<UTDDebuffComponent>(TEXT("StunDebuffComponent"));
	StunDebuffComponent->SetupAttachment(GetRootComponent());
	StunDebuffComponent->DebuffTag = FTDGameplayTags::GetTDGameplayTags().Debuff_Stun;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName(TEXT("WeaponHandSocket")));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ATDBaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATDBaseCharacter, bStunned);
	DOREPLIFETIME(ATDBaseCharacter, bBurned);
	DOREPLIFETIME(ATDBaseCharacter, bBeingShocked);
}

UAbilitySystemComponent* ATDBaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

TObjectPtr<UTDAbilitySystemComponent> ATDBaseCharacter::GetTDASC()
{
	if (IsValid(TDASC)) return TDASC;

	TDASC = Cast<UTDAbilitySystemComponent>(AbilitySystemComponent);

	return TDASC;
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

void ATDBaseCharacter::Die(const FVector& RagdollImpulse)
{
	Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));

	Multicast_ApplyDeath(RagdollImpulse); // ĳ���� ��� ó��. Server & Client
}

void ATDBaseCharacter::Multicast_ApplyDeath_Implementation(const FVector& RagdollImpulse) // ĳ���� ��� ó��
{
	if (GetMesh())
	{
		// ��� ���� ���
		if (DeathSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation(), GetActorRotation());
		}

		// ���� �ٴڿ� ����߸���
		if (Weapon)
		{
			Weapon->SetSimulatePhysics(true);
			Weapon->SetEnableGravity(true);
			Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
			Weapon->AddImpulse(RagdollImpulse * 0.05f, NAME_None, true);
		}

		// Ragdoll ������ ���� ĳ���� �Ž�/ĸ�� �����ϱ� 
		GetMesh()->SetSimulatePhysics(true);
		GetMesh()->SetEnableGravity(true);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
		GetMesh()->AddImpulse(RagdollImpulse, NAME_None, true); // Ragdoll ��� ����
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision); // Capsule �浹X

		// Ensure Debuff components are valid before deactivating them
		if (DotDamageDebuffComponent)
		{
			DotDamageDebuffComponent->Deactivate(); // ����� ����
		}
		if (StunDebuffComponent)
		{
			StunDebuffComponent->Deactivate();
		}
		bDead = true;
		OnDeathDelegate.Broadcast(this); // ��� Broadcast
	}
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
	if (false == HasAuthority()) return; // ������ �ƴϸ� return

	GetTDASC()->AddCharacterAbilities(StartupAbilities);
	GetTDASC()->AddCharacterPassiveAbilities(StartupPassiveAbilities);
}

void ATDBaseCharacter::StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bStunned = NewCount > 0;

	if (bStunned)
	{
		GetCharacterMovement()->MaxWalkSpeed = 0.f;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	}
}

float ATDBaseCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float DamageTaken = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	OnDamageDelegate.Broadcast(DamageTaken); // OnDamageDelegate ��ε�ĳ����.

	return DamageTaken;
}

void ATDBaseCharacter::OnRep_Stunned()
{
}

void ATDBaseCharacter::OnRep_Burned()
{
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

bool ATDBaseCharacter::IsDeadBP_Implementation() const
{
	return bDead;
}

bool ATDBaseCharacter::IsDead() const
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

FOnASCRegisteredSignature ATDBaseCharacter::GetOnASCRegisteredDelegate()
{
	return OnASCRegisteredDelegate;
}

FOnDeathSignature& ATDBaseCharacter::GetOnDeathDelegate()
{
	return OnDeathDelegate;
}

void ATDBaseCharacter::SetInShockLoop(bool bInLoop)
{
	return;
}

void ATDBaseCharacter::SetBeingShockedBP_Implementation(bool bInShock)
{
	bBeingShocked = bInShock;
}

void ATDBaseCharacter::SetBeingShocked(bool bInShock)
{
	bBeingShocked = bInShock;
}

bool ATDBaseCharacter::BeingShockedBP_Implementation() const
{
	return bBeingShocked;
}

bool ATDBaseCharacter::BeingShocked() const
{
	return bBeingShocked;
}

USkeletalMeshComponent* ATDBaseCharacter::GetWeaponBP_Implementation()
{
	return Weapon;
}

USkeletalMeshComponent* ATDBaseCharacter::GetWeapon()
{
	return Weapon;
}

void ATDBaseCharacter::SpawnWeapon()
{
}

void ATDBaseCharacter::IncremenetMinionCount_Implementation(int32 Amount)
{
	MinionCnt += Amount;
}

void ATDBaseCharacter::IncremenetMinionCntCPP(int32 Amount)
{
	MinionCnt += Amount;
}
