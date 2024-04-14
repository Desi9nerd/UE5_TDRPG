#pragma once
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IPlayer.generated.h"

UINTERFACE(MinimalAPI)
class UIPlayer : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TDRPG_API IIPlayer
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintNativeEvent)
	void AddToExp(int32 InExp);
	virtual void AddToExpCPP(int32 InExp);

	UFUNCTION(BlueprintNativeEvent)
	void LevelUp();
	virtual void LevelUpCPP();
};
