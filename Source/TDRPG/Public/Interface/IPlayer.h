#pragma once
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IPlayer.generated.h"

UINTERFACE(MinimalAPI)
class UIPlayer : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TDRPG_API IIPlayer
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintNativeEvent)
	void AddToExp(int32 InExp);
	virtual void AddToExpCPP(int32 InExp);

	UFUNCTION(BlueprintNativeEvent)
	void AddToPlayerLevelBP(int32 InPlayerLevel);
	virtual void AddToPlayerLevel(int32 InPlayerLevel);

	UFUNCTION(BlueprintNativeEvent)
	void AddToAttributePointsBP(int32 InAttributePoints);
	virtual void AddToAttributePoints(int32 InAttributePoints);

	UFUNCTION(BlueprintNativeEvent)
	void AddToSkillPointsBP(int32 InSkillPoints);
	virtual void AddToSkillPoints(int32 InSkillPoints);

	UFUNCTION(BlueprintNativeEvent)
	int32 FindLevelForExpBP(int32 InExp) const;
	virtual int32 FindLevelForExp(int32 InExp) const;

	UFUNCTION(BlueprintNativeEvent)
	int32 GetExpBP() const;
	virtual int32 GetExp() const;

	UFUNCTION(BlueprintNativeEvent)
	int32 GetAttributePointsBP() const;
	virtual int32 GetAttributePoints() const;

	UFUNCTION(BlueprintNativeEvent)
	int32 GetSkillPointsBP() const;
	virtual int32 GetSkillPoints() const;

	UFUNCTION(BlueprintNativeEvent)
	int32 GetAttributePointsRewardBP(int32 PlayerLevel) const;
	virtual int32 GetAttributePointsReward(int32 PlayerLevel) const;

	UFUNCTION(BlueprintNativeEvent)
	int32 GetSkillPointsRewardBP(int32 PlayerLevel) const;
	virtual int32 GetSkillPointsReward(int32 PlayerLevel) const;

	UFUNCTION(BlueprintNativeEvent)
	void LevelUp();
	virtual void LevelUpCPP();



	//TDPlayerController* GetTDPlayerControllerRef(APlayerController* InPlayerController);
};
