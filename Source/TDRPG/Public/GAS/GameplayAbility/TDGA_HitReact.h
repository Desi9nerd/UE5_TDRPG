#pragma once
#include "CoreMinimal.h"
#include "GAS/GameplayAbility/TDGA.h"
#include "TDGA_HitReact.generated.h"

/** 캐릭터가 피격되었을때 몽타주를 플레이 하는 GameplayAbility
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
