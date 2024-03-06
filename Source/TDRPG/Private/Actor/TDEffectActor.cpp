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
	// UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent�Լ��� GetAbilitySystemComponentFromActor�Լ��� �����ϰ� �� �������� const IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(Target)�� ������ ��ģ��. 
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target); 
	if (TargetASC == nullptr) return;

	check(GameplayEffectClass);

	// FGameplayEffectContextHandle�� FGameplayEffectSpecHandle�� ���� FGameplayEffectContext�� FGameplayEffectSpec�� ���δ� wrapper ����ü��.
	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, 1.f, EffectContextHandle);
	TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
}
