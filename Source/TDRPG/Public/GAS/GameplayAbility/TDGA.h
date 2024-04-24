#pragma once
#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "TDGA.generated.h"

/**
 * 
 */
UCLASS()
class TDRPG_API UTDGA : public UGameplayAbility
{
	GENERATED_BODY()

public:
	virtual FString GetDescription(int32 AbilityLevel);
	virtual FString GetNextAbilityLevelDescription(int32 AbilityLevel);
	static FString GetLockedDescription(int32 AbilityLevel);

	// InputTag는 게임 중 런타임에 변경될 수도 있다
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	FGameplayTag StartupInputTag; // 게임시작 시 InputTag

protected:
	float GetManaCost(float InAbilityLevel = 1.f) const;
	float GetCooldown(float InAbilityLevel = 1.f) const;
};
