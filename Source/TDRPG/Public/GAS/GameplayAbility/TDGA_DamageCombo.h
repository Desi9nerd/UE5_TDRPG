#pragma once
#include "CoreMinimal.h"
#include "GAS/GameplayAbility/TDGA_Damage.h"
#include "TDGA_DamageCombo.generated.h"

class UAnimMontage;

/**
 * 
 */
UCLASS()
class TDRPG_API UTDGA_DamageCombo : public UTDGA_Damage
{
	GENERATED_BODY()

public:
	UTDGA_DamageCombo();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	UFUNCTION()
	void OnCompleteCallback();

	UFUNCTION()
	void OnInterruptedCallback();

	FName GetNextSection();
	void StartComboTimer();
	void CheckComboInput();
	
	UPROPERTY(EditDefaultsOnly, Category = "Combo", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> ComboActionMontage;

	//********************************************************
	UPROPERTY(EditDefaultsOnly, Category = "Combo", Meta = (AllowPrivateAccess = "true"))
	FString MontageSectionNamePrefix;
	UPROPERTY(EditDefaultsOnly, Category = "Combo", Meta = (AllowPrivateAccess = "true"))
	uint8 MaxComboCount;
	UPROPERTY(EditDefaultsOnly, Category = "Combo", Meta = (AllowPrivateAccess = "true"))
	float InputThreashold = 0.1f;
	//********************************************************

	uint8 CurrentCombo = 0;
	FTimerHandle ComboTimerHandle;
	bool HasNextComboInput = false;

	bool bFirstCombo = true;
};
