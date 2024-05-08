#pragma once
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NiagaraComponent.h"
#include "TDDebuffComponent.generated.h"

class UAbilitySystemComponent;

/**
 * 
 */
UCLASS()
class TDRPG_API UTDDebuffComponent : public UNiagaraComponent
{
	GENERATED_BODY()

public:
	UTDDebuffComponent();

	UPROPERTY(VisibleAnywhere)
	FGameplayTag DebuffTag;

protected:
	virtual void BeginPlay() override;
	void DebuffTagChangedByCount(const FGameplayTag CallbackTag, int32 NewCount);

private:
	UFUNCTION()
	void OnASCRegister(UAbilitySystemComponent* InASC);

	UFUNCTION()
	void OnOwnerDeath(AActor* DeadActor);
};