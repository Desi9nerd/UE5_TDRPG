#include "Character/TDBaseCharacter.h"
#include "AbilitySystemComponent.h"

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

void ATDBaseCharacter::InitializeStatAttributes() const
{
	check(IsValid(GetAbilitySystemComponent())); // ASC 유무 검사
	check(DefaultStatAttributes);

	const FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(DefaultStatAttributes, 1.f, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
}
