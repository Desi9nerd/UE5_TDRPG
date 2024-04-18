#pragma once
#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "TDPlayerState.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;
class UTDDA_LevelUp;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChanged, int32 /* StatValue */)

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
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	void AddToPlayerLevel(int32 InPlayerLevel);
	void AddToExp(int32 InExp);
	void SetPlayerLevel(int32 InPlayerLevel);
	void SetExp(int32 InExp);

	FORCEINLINE int32 GetPlayerLevel() const { return PlayerLevel; }
	FORCEINLINE int32 GetExp() const { return Exp; }
	FORCEINLINE int32 GetAttributePoints() const { return AttributePoints; }
	FORCEINLINE int32 GetSkillPoints() const { return SkillPoints; }

	FOnPlayerStatChanged OnExpChangedDelegate;
	FOnPlayerStatChanged OnPlayerLevelChangedDelegate;
	FOnPlayerStatChanged OnAttributePointsChangedDelegate;
	FOnPlayerStatChanged OnSkillPointsChangedDelegate;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UTDDA_LevelUp> TDDA_LevelUpInfo;

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

private:
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_PlayerLevel)
	int32 PlayerLevel = 1;
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_Exp)
	int32 Exp = 1;
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_AttributePoints)
	int32 AttributePoints = 1;
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_SkillPoints)
	int32 SkillPoints = 1;

	UFUNCTION()
	void OnRep_PlayerLevel();
	UFUNCTION()
	void OnRep_Exp();
	UFUNCTION()
	void OnRep_AttributePoints(int32 InNewAttributePoints);
	UFUNCTION()
	void OnRep_SkillPoints(int32 InNewSkillPoints);
};