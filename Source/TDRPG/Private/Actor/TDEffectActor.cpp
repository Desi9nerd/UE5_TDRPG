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

void ATDEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	if (TargetActor->ActorHasTag(FName("Enemy")) && false == bApplyEffectsToEnemies) return; // �������� Effect�� �������� �ʾƾ��ϴ� ���̶�� ����ó��.

	// UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent�Լ��� GetAbilitySystemComponentFromActor�Լ��� �����ϰ� �� �������� const IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(Target)�� ������ ��ģ��. 
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (TargetASC == nullptr) return;

	check(GameplayEffectClass);

	// FGameplayEffectContextHandle�� FGameplayEffectSpecHandle�� ���� FGameplayEffectContext�� FGameplayEffectSpec�� ���δ� wrapper ����ü��.
	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, ActorLevel, EffectContextHandle);
	const FActiveGameplayEffectHandle ActiveEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

	// EffectSpecHandle.Data�� Spec�̴�. EffectSpecHandle.Data.Get()->Def�� GE��.
	// GE�� DurationPolicy�� Infinite��� true
	const bool bIsInfinite = EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite; 
	if (bIsInfinite && InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		ActiveEffectHandles.Add(ActiveEffectHandle, TargetASC); // TMap�� GE Handle�� TargetActor�� ASC�� ����
	}
	else if (false == bIsInfinite)
	{
		Destroy();
	}
}

void ATDEffectActor::OnOverlap(AActor* TargetActor)
{
	if (TargetActor->ActorHasTag(FName("Enemy")) && false == bApplyEffectsToEnemies) return; // ����ó��.

	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}
	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}
	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}
}

void ATDEffectActor::OnEndOverlap(AActor* TargetActor)
{
	if (TargetActor->ActorHasTag(FName("Enemy")) && false == bApplyEffectsToEnemies) return; // ����ó��.

	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}
	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}
	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}
	if (InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		if (false == IsValid(TargetASC)) return;

		TArray<FActiveGameplayEffectHandle> HandlesToRemove; // ������ ��ϵ��� ��� TArray ����
		for (TTuple<FActiveGameplayEffectHandle, UAbilitySystemComponent*> HandlePair : ActiveEffectHandles)
		{
			if (TargetASC == HandlePair.Value) // TargetActor�� ASC�� ActiveEffectHandles TMap�� �ִٸ�
			{
				TargetASC->RemoveActiveGameplayEffect(HandlePair.Key, 1); // GE 1���� ����
				HandlesToRemove.Add(HandlePair.Key); // ������ �͵� ����
			}
		}
		for (FActiveGameplayEffectHandle& Handle : HandlesToRemove)
		{
			ActiveEffectHandles.FindAndRemoveChecked(Handle); // ActiveEffectHandles TMap ��Ͽ��� ����
		}
	}
}
