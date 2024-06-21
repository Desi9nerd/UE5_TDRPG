#pragma once
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IPlayer.generated.h"

UINTERFACE(MinimalAPI)
class UIPlayer : public UInterface
{
	GENERATED_BODY()
};

/** 플레이어 관련 인터페이스
 * 
 */
class TDRPG_API IIPlayer
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintNativeEvent)
	void AddToExp(int32 InExp);
	virtual void AddToExpCPP(int32 InExp) = 0;

	UFUNCTION(BlueprintNativeEvent)
	void AddToPlayerLevelBP(int32 InPlayerLevel);
	virtual void AddToPlayerLevel(int32 InPlayerLevel) = 0;

	UFUNCTION(BlueprintNativeEvent)
	void AddToAttributePointsBP(int32 InAttributePoints);
	virtual void AddToAttributePoints(int32 InAttributePoints) = 0;

	UFUNCTION(BlueprintNativeEvent)
	void AddToSkillPointsBP(int32 InSkillPoints);
	virtual void AddToSkillPoints(int32 InSkillPoints) = 0;

	UFUNCTION(BlueprintNativeEvent)
	int32 FindLevelForExpBP(int32 InExp) const;
	virtual int32 FindLevelForExp(int32 InExp) const = 0;

	UFUNCTION(BlueprintNativeEvent)
	int32 GetExpBP() const;
	virtual int32 GetExp() const = 0;

	UFUNCTION(BlueprintNativeEvent)
	int32 GetAttributePointsBP() const;
	virtual int32 GetAttributePoints() const = 0;

	UFUNCTION(BlueprintNativeEvent)
	int32 GetSkillPointsBP() const;
	virtual int32 GetSkillPoints() const = 0;

	UFUNCTION(BlueprintNativeEvent)
	int32 GetAttributePointsRewardBP(int32 PlayerLevel) const;
	virtual int32 GetAttributePointsReward(int32 PlayerLevel) const = 0;

	UFUNCTION(BlueprintNativeEvent)
	int32 GetSkillPointsRewardBP(int32 PlayerLevel) const;
	virtual int32 GetSkillPointsReward(int32 PlayerLevel) const = 0;

	UFUNCTION(BlueprintNativeEvent)
	void LevelUp();
	virtual void LevelUpCPP() = 0;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ShowDecalBP(); // 추후에 제거
	virtual void ShowDecal() = 0;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void HideDecalBP();
	virtual void HideDecal() = 0;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SaveProgressBP(const FName& CheckpointTag);
	virtual void SaveProgress(const FName& CheckpointTag) = 0;

	//TDPlayerController* GetTDPlayerControllerRef(APlayerController* InPlayerController);
};
