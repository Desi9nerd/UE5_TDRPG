#pragma once
#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "TDAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class TDRPG_API UTDAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	static UTDAssetManager& GetTDAssetManager();

protected:
	virtual void StartInitialLoading() override;

};
