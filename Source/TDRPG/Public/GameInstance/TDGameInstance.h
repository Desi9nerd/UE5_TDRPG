#pragma once
#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "TDGameInstance.generated.h"

/**
 * 
 */

UCLASS()
class TDRPG_API UTDGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FName PlayerStartTag = FName();

	UPROPERTY()
	FString LoadSlotName = FString();

	UPROPERTY()
	int32 LoadSlotIndex = 0;

};
