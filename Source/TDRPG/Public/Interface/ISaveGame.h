#pragma once
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ISaveGame.generated.h"

UINTERFACE(MinimalAPI)
class UISaveGame : public UInterface
{
	GENERATED_BODY()
};

/** 게임 저장과 관련된 인터페이스
 *  TDPlayerStart, TDSpawnVolumeActor에서 사용.
 */
class TDRPG_API IISaveGame
{
	GENERATED_BODY()
public:
	virtual bool ShouldLoadTransform() = 0;
	virtual void LoadActor() = 0;
};
