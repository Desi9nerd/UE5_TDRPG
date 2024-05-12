#pragma once
#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "TDGA.generated.h"

class UTDAT_TargetData;
/**
 * 
 */
UCLASS()
class TDRPG_API UTDGA : public UGameplayAbility
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual FString GetDescription(int32 AbilityLevel);
	virtual FString GetNextAbilityLevelDescription(int32 AbilityLevel);
	static FString GetLockedDescription(int32 AbilityLevel);

	// InputTag는 게임 중 런타임에 변경될 수도 있다
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	FGameplayTag StartupInputTag; // 게임시작 시 InputTag

protected:
	float GetManaCost(float InAbilityLevel = 1.f) const;
	float GetCooldown(float InAbilityLevel = 1.f) const;

	TObjectPtr<UTDAT_TargetData> TargetDataAbilityTask;
};
