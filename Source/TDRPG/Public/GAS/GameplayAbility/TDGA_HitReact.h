#pragma once
#include "CoreMinimal.h"
#include "GAS/GameplayAbility/TDGA.h"
#include "TDGA_HitReact.generated.h"

/** ĳ���Ͱ� �ǰݵǾ����� ��Ÿ�ָ� �÷��� �ϴ� GameplayAbility
 * 
 */
UCLASS()
class TDRPG_API UTDGA_HitReact : public UTDGA
{
	GENERATED_BODY()

public:
	UTDGA_HitReact();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
protected:
	UFUNCTION()
	void RemoveGameplayEffectFromOwnerWithHandle();
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayEffect> GameplayEffectClass;

	UPROPERTY()
	FActiveGameplayEffectHandle ActiveGEHitReact;
};
