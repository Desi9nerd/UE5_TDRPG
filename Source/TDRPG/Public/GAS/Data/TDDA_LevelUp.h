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

	// �������� �ʿ��� ����ġ �ʿ䷮
	UPROPERTY(EditDefaultsOnly)
	int32 LevelUpRequirement = 0;

	// ������ �� �ִ� ����Ʈ
	UPROPERTY(EditDefaultsOnly)
	int32 AttributePointAward = 1; // Attribute ����Ʈ
	UPROPERTY(EditDefaultsOnly)
	int32 SkillPointAward = 1; // Skill ����Ʈ
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
