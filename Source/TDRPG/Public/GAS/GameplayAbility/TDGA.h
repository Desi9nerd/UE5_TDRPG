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
	virtual FString GetDescription(int32 Level);
	virtual FString GetNextLevelDescription(int32 Level);
	static FString GetLockedDescription(int32 Level);

	// InputTag는 게임 중 런타임에 변경될 수도 있다
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	FGameplayTag StartupInputTag; // 게임시작 시 InputTag
};
