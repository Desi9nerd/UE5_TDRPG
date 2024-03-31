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
	if (TargetActor->ActorHasTag(FName("Enemy")) && false == bApplyEffectsToEnemies) return; // 적이지만 Effect를 적용하지 않아야하는 적이라면 예외처리.

	// UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent함수는 GetAbilitySystemComponentFromActor함수를 리턴하고 이 과정에서 const IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(Target)의 과정을 거친다. 
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (TargetASC == nullptr) return;

	check(GameplayEffectClass);

	// FGameplayEffectContextHandle와 FGameplayEffectSpecHandle는 각각 FGameplayEffectContext와 FGameplayEffectSpec를 감싸는 wrapper 구조체다.
	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, ActorLevel, EffectContextHandle);
	const FActiveGameplayEffectHandle ActiveEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

	// EffectSpecHandle.Data는 Spec이다. EffectSpecHandle.Data.Get()->Def는 GE다.
	// GE의 DurationPolicy가 Infinite라면 true
	const bool bIsInfinite = EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite; 
	if (bIsInfinite && InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		ActiveEffectHandles.Add(ActiveEffectHandle, TargetASC); // TMap에 GE Handle과 TargetActor의 ASC를 담음
	}
	else if (false == bIsInfinite)
	{
		Destroy();
	}
}

void ATDEffectActor::OnOverlap(AActor* TargetActor)
{
	if (TargetActor->ActorHasTag(FName("Enemy")) && false == bApplyEffectsToEnemies) return; // 예외처리.

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
	if (TargetActor->ActorHasTag(FName("Enemy")) && false == bApplyEffectsToEnemies) return; // 예외처리.

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

		TArray<FActiveGameplayEffectHandle> HandlesToRemove; // 제거할 목록들을 담는 TArray 변수
		for (TTuple<FActiveGameplayEffectHandle, UAbilitySystemComponent*> HandlePair : ActiveEffectHandles)
		{
			if (TargetASC == HandlePair.Value) // TargetActor의 ASC가 ActiveEffectHandles TMap에 있다면
			{
				TargetASC->RemoveActiveGameplayEffect(HandlePair.Key, 1); // GE 1스택 제거
				HandlesToRemove.Add(HandlePair.Key); // 제거할 것들 담음
			}
		}
		for (FActiveGameplayEffectHandle& Handle : HandlesToRemove)
		{
			ActiveEffectHandles.FindAndRemoveChecked(Handle); // ActiveEffectHandles TMap 목록에서 제거
		}
	}
}
