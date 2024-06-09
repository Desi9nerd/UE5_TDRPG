#pragma once
#include "CoreMinimal.h"
#include "GameplayAbilitySpec.h"
#include "TDBaseCharacter.h"
#include "Interface/IPlayer.h"
#include "TDCharacter.generated.h"

class ATDGameModeBase;
class ATDPlayerController;
class ATDPlayerState;
class USpringArmComponent;
class UCameraComponent;
class UNiagaraComponent;
class UTDInventoryComponent;
class USphereComponent;

/**
 * 
 */
UCLASS()
class TDRPG_API ATDCharacter : public ATDBaseCharacter, public IIPlayer
{
	GENERATED_BODY()

public:
	ATDCharacter();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const;
	virtual void PossessedBy(AController* NewController) override; // Server
	void LoadProgress();
	virtual void OnRep_PlayerState() override; // Client
	FORCEINLINE TObjectPtr<UTDInventoryComponent> GetInventoryComponent();
	FORCEINLINE TObjectPtr<ATDPlayerController> GetTDPlayerController();
	FORCEINLINE TObjectPtr<ATDGameModeBase> GetTDGameModeBase();
	FORCEINLINE TObjectPtr<ATDPlayerState> GetTDPlayerState();

	//** ICombat *********************************************
	//virtual int32 GetPlayerLevelBP_Implementation() override;
	virtual int32 GetPlayerLevel() override;
	//********************************************************

	//** IPlayer *********************************************
	//virtual void AddToExp_Implementation(int32 InExp) override;
	virtual void AddToExpCPP(int32 InExp) override;
	//virtual void LevelUp_Implementation() override;
	virtual void LevelUpCPP() override;
	virtual void AddToPlayerLevel(int32 InPlayerLevel) override;
	virtual void AddToPlayerLevelBP_Implementation(int32 InPlayerLevel) override;
	virtual void AddToAttributePoints(int32 InAttributePoints) override;
	virtual void AddToAttributePointsBP_Implementation(int32 InAttributePoints) override;
	virtual void AddToSkillPoints(int32 InSkillPoints) override;
	virtual void AddToSkillPointsBP_Implementation(int32 InSpellPoints) override;

	virtual int32 GetExp() const override;
	virtual int32 GetExpBP_Implementation() const override;
	virtual int32 FindLevelForExp(int32 InExp) const override;
	virtual int32 FindLevelForExpBP_Implementation(int32 InExp) const override;
	virtual int32 GetAttributePointsReward(int32 PlayerLevel) const override;
	virtual int32 GetAttributePointsRewardBP_Implementation(int32 PlayerLevel) const override;
	virtual int32 GetSkillPointsReward(int32 PlayerLevel) const override;
	virtual int32 GetSkillPointsRewardBP_Implementation(int32 PlayerLevel) const override;
	virtual int32 GetAttributePoints() const override;
	virtual int32 GetAttributePointsBP_Implementation() const override;
	virtual int32 GetSkillPoints() const override;
	virtual int32 GetSkillPointsBP_Implementation() const override;
	virtual void ShowDecalBP_Implementation() override;
	virtual void ShowDecal() override;
	virtual void HideDecalBP_Implementation() override;
	virtual void HideDecal() override;
	virtual void SaveProgressBP_Implementation(const FName& CheckpointTag) override;
	virtual void SaveProgress(const FName& CheckpointTag) override;
	//********************************************************
	void SaveGameSaveAbilities(const FGameplayAbilitySpec& AbilitySpec);

	virtual void OnRep_Stunned() override;
	virtual void OnRep_Burned() override;

protected:
	UPROPERTY(VisibleAnywhere, Category = Camera)
	TObjectPtr<USpringArmComponent> CameraSpringArm;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	TObjectPtr<UCameraComponent> FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UTDInventoryComponent> TDInventoryComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UNiagaraComponent> Niagara_LevelUp;

private:
	virtual void InitAbilityActorInfo() override;

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_LevelUpParticleEffect() const;


	UPROPERTY()
	TObjectPtr<ATDGameModeBase> TDGameModeBase;

	UPROPERTY()
	TObjectPtr<ATDPlayerState> TDPlayerState;

	UPROPERTY()
	TObjectPtr<ATDPlayerController> TDPlayerController;
};