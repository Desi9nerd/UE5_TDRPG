#pragma once

#include "CoreMinimal.h"
#include "TDBaseCharacter.h"
#include "Interface/IPlayer.h"
#include "TDCharacter.generated.h"

class ATDPlayerState;

/**
 * 
 */
UCLASS()
class TDRPG_API ATDCharacter : public ATDBaseCharacter, public IIPlayer
{
	GENERATED_BODY()

public:
	ATDCharacter();
	virtual void PossessedBy(AController* NewController) override; // Server
	virtual void OnRep_PlayerState() override; // Client

	//** ICombat *********************************************
	//virtual int32 GetPlayerLevelBP_Implementation() override;
	virtual int32 GetPlayerLevel() override;
	//********************************************************

	//** IPlayer *********************************************
	//virtual void AddToExp_Implementation(int32 InExp) override;
	virtual void AddToExpCPP(int32 InExp) override;
	//virtual void LevelUp_Implementation() override;
	virtual void LevelUpCPP() override;
	//********************************************************

protected:
	UPROPERTY(VisibleAnywhere, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraSpringArm;

	UPROPERTY(VisibleAnywhere, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;

private:
	virtual void InitAbilityActorInfo() override;

	UPROPERTY()
	TObjectPtr<ATDPlayerState> TDPlayerState;
};