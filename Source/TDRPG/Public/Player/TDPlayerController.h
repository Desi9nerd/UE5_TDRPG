#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "TDPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class IIEnemy;
class UTDDAInput;
class UTDAbilitySystemComponent;
class USplineComponent;

/**
 * 
 */

UCLASS()
class TDRPG_API ATDPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATDPlayerController();
	virtual void PlayerTick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	void Move(const FInputActionValue& InputActionValue);
	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);
	TObjectPtr<UTDAbilitySystemComponent> GetASC();
	void AutoRun();
	void ShiftPressed() { bShiftKeyDown = true; };
	void ShiftReleased() { bShiftKeyDown = false; };

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> RPGContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> ShiftAction;
	bool bShiftKeyDown = false;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UTDDAInput> InputData;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UTDAbilitySystemComponent> TDASC;

	UPROPERTY()
	TObjectPtr<APawn> PlayerControlledPawn;

	//** 이동
	UPROPERTY(EditDefaultsOnly)
	float AutoRunAcceptanceRadius = 50.f;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline; // 이동경로 Spline

	FVector CachedDestination = FVector::ZeroVector;
	float FollowTime = 0.f;
	float ShortPressThreshold = 0.5f;
	bool bAutoRunning = false;
	bool bTargeting = false;
	
	void CursorTrace(); // 마우스 CursorTrace
	FHitResult CursorHit; // 마우스 CursorHit
	IIEnemy* LastActor;
	IIEnemy* ThisActor;

};
