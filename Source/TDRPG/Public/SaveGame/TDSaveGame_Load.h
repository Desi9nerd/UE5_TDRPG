﻿#pragma once
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
	//********************************************************
	//** SaveGame 데이터.
	UPROPERTY()
	FString SlotName = FString();

	UPROPERTY()
	int32 SlotIndex = 0;

	UPROPERTY()
	FString PlayerName = FString("Default Player Name");
	UPROPERTY()
	FString MapName = FString("Default Map Name");

	UPROPERTY()
	FName PlayerStartTag;

	UPROPERTY()
	TEnumAsByte<ESG_SaveSlotStatus> SaveSlotStatus = Vacant;

	UPROPERTY()
	bool bFirstTimeLoadIn = true; // SaveData 생성 후 처음 게임을 시작하는지/아닌지.
	//********************************************************

	//********************************************************
	//** Player 정보.
	UPROPERTY()
	int32 PlayerLevel = 1;
	UPROPERTY()
	int32 Exp = 0;
	UPROPERTY()
	int32 SkillPoints = 0;
	UPROPERTY()
	int32 AttributePoints = 0;
	UPROPERTY()
	float Strength = 0;
	UPROPERTY()
	float Intelligence = 0;
	UPROPERTY()
	float Resilience = 0;
	UPROPERTY()
	float Vigor = 0;
	//********************************************************

};
