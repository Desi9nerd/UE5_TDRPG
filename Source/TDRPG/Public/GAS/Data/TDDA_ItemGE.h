#pragma once
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TDDA_ItemGE.generated.h"

class UGameplayEffect;

/**
 * 
 */

UCLASS()
class TDRPG_API UTDDA_ItemGE : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TMap<FName, TSubclassOf<UGameplayEffect>> ItemGEClassList;
	
};
