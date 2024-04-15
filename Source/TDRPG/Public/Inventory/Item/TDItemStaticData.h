#pragma once
#include "CoreMinimal.h"
#include "TDItemStaticData.generated.h"

/**
 * 
 */

UCLASS(BlueprintType, Blueprintable)
class UTDItemStaticData : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName Name;
};
