#pragma once
#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "TDGameInstance.generated.h"

/** GameInstance - 게임저장 기능을 위해 PlayerStartTag, LoadSlotName, LoadSlotIndex를 가지고 있음.
 *  게임이 실행되는 동안 전역적으로 유지되며, 다양한 레벨과 게임 모드 간에 데이터를 공유함.
 *  레벨이 전환되어도 GameInstance는 삭제되지 않고 유지됨.
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
