#pragma once
#include "CoreMinimal.h"
#include "GAS/GameplayAbility/TDGA_Damage.h"
#include "TDGA_DamageFrozenField.generated.h"

/**
 * 
 */
UCLASS()
class TDRPG_API UTDGA_DamageFrozenField : public UTDGA_Damage
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};
