#include "Actor/TDEffectActor.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

ATDEffectActor::ATDEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(GetRootComponent());
}

void ATDEffectActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATDEffectActor::ApplyEffectToTarget(AActor* Target, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	// UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent함수는 GetAbilitySystemComponentFromActor함수를 리턴하고 이 과정에서 const IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(Target)의 과정을 거친다. 
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target); 
	if (TargetASC == nullptr) return;

	check(GameplayEffectClass);

	// FGameplayEffectContextHandle와 FGameplayEffectSpecHandle는 각각 FGameplayEffectContext와 FGameplayEffectSpec를 감싸는 wrapper 구조체다.
	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, 1.f, EffectContextHandle);
	TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
}
