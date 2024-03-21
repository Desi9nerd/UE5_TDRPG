#include "Character/TDBaseCharacter.h"
#include "AbilitySystemComponent.h"
#include "GAS/TDAbilitySystemComponent.h"

ATDBaseCharacter::ATDBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	//** Mesh
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

UAbilitySystemComponent* ATDBaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
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
	check(IsValid(GetAbilitySystemComponent())); // ASC ���� �˻�
	check(GameplayEffectClass); // GameplayEffectClass �Ҵ翩�� �˻�(�����Ϳ��� �Ҵ� �� �ϸ� ����)

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
}

FVector ATDBaseCharacter::GetCombatSocketLocation() // ���� ��ġ�� ����
{
	check(Weapon);

	return Weapon->GetSocketLocation(WeaponTipSocketName);
}
