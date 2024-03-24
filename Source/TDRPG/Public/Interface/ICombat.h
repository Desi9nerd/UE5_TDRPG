#pragma once
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ICombat.generated.h"

UINTERFACE(MinimalAPI, BlueprintType)
class UICombat : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TDRPG_API IICombat
{
	GENERATED_BODY()

public:
	virtual int32 GetPlayerLevel();
	virtual FVector GetCombatSocketLocation(); // 소켓 위치를 리턴

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable) // BP ver.
	void UpdateFacingTarget(const FVector& Target);
	UFUNCTION() // C++ ver.
	virtual void SetFacingTarget(const FVector& FacingTarget);
};
