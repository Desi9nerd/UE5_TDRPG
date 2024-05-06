#pragma once
#include "CoreMinimal.h"
#include "GAS/GameplayAbility/TDGA.h"
#include "TDGA_Dash.generated.h"

/**
 * 
 */
UCLASS()
class TDRPG_API UTDGA_Dash : public UTDGA
{
	GENERATED_BODY()

public:
	UTDGA_Dash();
	void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

private:
	UFUNCTION()
	void OnEndAbility();

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess))
	TObjectPtr<UAnimMontage> DashMontageToPlay;
};
