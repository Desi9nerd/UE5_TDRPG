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
	
	virtual void SetFacingTarget(const FVector& FacingTarget) override; // 현재 BP버전을 추후에 이것으로 적용예정

protected:
	UPROPERTY(VisibleAnywhere, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraSpringArm;

	UPROPERTY(VisibleAnywhere, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;

	UPROPERTY(VisibleAnywhere, Category = "Motion Warping Component")
	TObjectPtr<class UMotionWarpingComponent> MotionWarpingComponent;

private:
	virtual void InitAbilityActorInfo() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Motion Warping Component", Meta = (AllowPrivateAccess = "true"))
	FName WarpTargetName = FName("FacingTarget");
};