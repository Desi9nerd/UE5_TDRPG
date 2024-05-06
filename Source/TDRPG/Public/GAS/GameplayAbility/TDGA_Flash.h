#pragma once
#include "CoreMinimal.h"
#include "GAS/GameplayAbility/TDGA.h"
#include "TDGA_Flash.generated.h"

class UAbilityTask_WaitGameplayEvent;
/**
 * 
 */
UCLASS()
class TDRPG_API UTDGA_Flash : public UTDGA
{
	GENERATED_BODY()

public:
	UTDGA_Flash();
	void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

private:
	UFUNCTION()
	void OnFlashTeleport(FGameplayEventData Payload);

	UFUNCTION()
	void OnEndAbility();

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess))
	TObjectPtr<UAnimMontage> FlashMontageToPlay;
	
};
