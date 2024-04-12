#pragma once
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TDDA_LevelUp.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FDA_LevelUp
{
	GENERATED_BODY()

	// 레벨업에 필요한 경험치 필요량
	UPROPERTY(EditDefaultsOnly)
	int32 LevelUpRequirement = 0;

	// 레벨업 당 주는 포인트
	UPROPERTY(EditDefaultsOnly)
	int32 AttributePointAward = 1; // Attribute 포인트
	UPROPERTY(EditDefaultsOnly)
	int32 SkillPointAward = 1; // Skill 포인트
};

UCLASS()
class TDRPG_API UTDDA_LevelUp : public UDataAsset
{
	GENERATED_BODY()

public:
	int32 FindDA_LevelUpForExp(int32 Exp);

	UPROPERTY(EditDefaultsOnly)
	TArray<FDA_LevelUp> DA_LevelUpInfo;
};
