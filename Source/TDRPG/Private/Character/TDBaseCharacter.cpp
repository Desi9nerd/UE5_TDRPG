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

void ATDBaseCharacter::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const // GE 적용하기
{
	check(IsValid(GetAbilitySystemComponent())); // ASC 유무 검사
	check(GameplayEffectClass); // GameplayEffectClass 할당여부 검사(에디터에서 할당 안 하면 터짐)

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

FVector ATDBaseCharacter::GetCombatSocketLocation() // 소켓 위치를 리턴
{
	check(Weapon);

	return Weapon->GetSocketLocation(WeaponTipSocketName);
}
