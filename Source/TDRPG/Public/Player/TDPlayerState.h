﻿#pragma once
#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "TDPlayerState.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;
class UTDDA_LevelUp;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChanged, int32 /* StatValue */);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnPlayerLevelChanged, int32 /*StatValue*/, bool /*bLevelUp*/);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAbilitySystemComponentInitialized);

/**
 * 
 */

UCLASS()
class TDRPG_API ATDPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ATDPlayerState();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	void AddToPlayerLevel(int32 InPlayerLevel);
	void AddToExp(int32 InExp);
	void AddToAttributePoints(int32 InNewAttributePoints);
	void AddToSkillPoints(int32 InNewSkillPoints);

	int32 GetPlayerLevel() const { return PlayerLevel; }
	int32 GetExp() const { return Exp; }
	int32 GetAttributePoints() const { return AttributePoints; }
	int32 GetSkillPoints() const { return SkillPoints; }
	void SetPlayerLevel(int32 InPlayerLevel);
	void SetExp(int32 InExp);
	void SetAttributePoints(int32 InAttributePoints);
	void SetSkillPoints(int32 InSkillPoints);

	FOnPlayerStatChanged OnExpChangedDelegate;
	FOnPlayerStatChanged OnAttributePointsChangedDelegate;
	FOnPlayerStatChanged OnSkillPointsChangedDelegate;
	FOnPlayerLevelChanged OnPlayerLevelChangedDelegate; // true면 레벨업, false면 레벨업X

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UTDDA_LevelUp> TDDA_LevelUpInfo;

	UPROPERTY(BlueprintAssignable, Category = "Abilities")
	FOnAbilitySystemComponentInitialized OnAbilitySystemComponentInitialized;

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

private:
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_PlayerLevel)
	int32 PlayerLevel = 1;
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_Exp)
	int32 Exp = 0;
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_AttributePoints)
	int32 AttributePoints = 0;
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_SkillPoints)
	int32 SkillPoints = 0;

	UFUNCTION()
	void OnRep_PlayerLevel();
	UFUNCTION()
	void OnRep_Exp();
	UFUNCTION()
	void OnRep_AttributePoints();
	UFUNCTION()
	void OnRep_SkillPoints();
};

