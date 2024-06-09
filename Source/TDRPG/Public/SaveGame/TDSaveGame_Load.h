#pragma once
#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "TDSaveGame_Load.generated.h"

/** 슬롯의 이름, 인덱스, 플레이어 이름 설정
 *  게임 데이터를 저장함.
 */

UENUM(BlueprintType)
enum ESG_SaveSlotStatus
{
	Vacant = 0,
	EnterName = 1,
	Taken = 2
};

UCLASS()
class TDRPG_API UTDSaveGame_Load : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	FString SlotName = FString();

	UPROPERTY()
	int32 SlotIndex = 0;

	UPROPERTY()
	FString PlayerName = FString("Default Player Name");
	UPROPERTY()
	FString MapName = FString("Default Map Name");

	UPROPERTY()
	TEnumAsByte<ESG_SaveSlotStatus> SaveSlotStatus = Vacant;
};
