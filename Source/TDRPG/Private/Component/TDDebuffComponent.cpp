#include "Component/TDDebuffComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Interface/ICombat.h"

UTDDebuffComponent::UTDDebuffComponent()
{
	bAutoActivate = false;
}

void UTDDebuffComponent::BeginPlay()
{
	Super::BeginPlay();

	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner());
	IICombat* CombatInterface = Cast<IICombat>(GetOwner());
	if (IsValid(ASC))
	{
		ASC->RegisterGameplayTagEvent(DebuffTag, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &ThisClass::DebuffTagChangedByCount);
	}
	else if (CombatInterface)
	{
		//CombatInterface->GetOnASCRegisteredDelegate().AddUObject(this, &ThisClass::OnASCRegister);

		CombatInterface->GetOnASCRegisteredDelegate().AddWeakLambda(this, [this](UAbilitySystemComponent* InASC)
			{
				InASC->RegisterGameplayTagEvent(DebuffTag, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &ThisClass::DebuffTagChangedByCount);
			});
	}

	if (CombatInterface)
	{
		CombatInterface->GetOnDeathDelegate().AddDynamic(this, &ThisClass::OnOwnerDeath);
	}
}

void UTDDebuffComponent::DebuffTagChangedByCount(const FGameplayTag CallbackTag, int32 NewCount)
{
	IICombat* CombatInterface = Cast<IICombat>(GetOwner());

	if (NewCount > 0 && CombatInterface && (false == CombatInterface->IsDead()) ) // NewCount>0이고 Owner가 죽지 않았을때
	{
		Activate();
	}
	else
	{
		Deactivate();
	}
}

void UTDDebuffComponent::OnASCRegister(UAbilitySystemComponent* InASC)
{
	InASC->RegisterGameplayTagEvent(DebuffTag, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &ThisClass::DebuffTagChangedByCount);
}

void UTDDebuffComponent::OnOwnerDeath(AActor* DeadActor)
{
	Deactivate();
}
