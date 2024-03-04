#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "RPGCharacter.generated.h"

/**
 * 
 */
UCLASS()
class TDRPG_API ARPGCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	ARPGCharacter();
	virtual void PossessedBy(AController* NewController) override; // Server
	virtual void OnRep_PlayerState() override; // Client

protected:
	UPROPERTY(VisibleAnywhere, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraSpringArm;

	UPROPERTY(VisibleAnywhere, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;

private:
	void InitAbilityActorInfo();
};