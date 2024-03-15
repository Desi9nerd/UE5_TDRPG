#pragma once

#include "CoreMinimal.h"
#include "TDBaseCharacter.h"
#include "TDCharacter.generated.h"

/**
 * 
 */
UCLASS()
class TDRPG_API ATDCharacter : public ATDBaseCharacter
{
	GENERATED_BODY()

public:
	ATDCharacter();
	virtual void PossessedBy(AController* NewController) override; // Server
	virtual void OnRep_PlayerState() override; // Client

	virtual int32 GetPlayerLevel() override;

protected:
	UPROPERTY(VisibleAnywhere, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraSpringArm;

	UPROPERTY(VisibleAnywhere, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;

private:
	virtual void InitAbilityActorInfo() override;
};