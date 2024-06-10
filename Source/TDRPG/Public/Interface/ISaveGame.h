#pragma once
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ISaveGame.generated.h"

UINTERFACE(MinimalAPI)
class UISaveGame : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TDRPG_API IISaveGame
{
	GENERATED_BODY()
public:
	virtual bool ShouldLoadTransform() = 0;
	virtual void LoadActor() = 0;
};
