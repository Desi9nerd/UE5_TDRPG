#pragma once
#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "GAS/Data/TDDA_CharacterClass.h"
#include "TDTargetPoint_Spawn.generated.h"

class ATDEnemyCharacter;

/** 적 몬스터를 스폰하는 역할의 TargetPoint
 *  스폰 시킬 몬스터 클래스를 등록해서 사용. 등록된 몬스터가 TargetPoint 위치에 스폰됨.
 */
UCLASS()
class TDRPG_API ATDTargetPoint_Spawn : public ATargetPoint
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SpawnEnemy();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ATDEnemyCharacter> EnemyCharacterClass; // 스폰시킬 몬스터 클래스.

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 EnemyLevel = 1; // 적 몬스터 레벨

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ECharacterClass CharacterClass = ECharacterClass::Warrior; // 적 몬스터 종류
};
