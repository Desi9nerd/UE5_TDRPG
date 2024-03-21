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

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> RPGContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UTDDAInput> InputData;


	//** ���콺�� ����Ű�°��� Enemy�� ������ �ܰ��� Highlight
	void CursorTrace(); // ���콺 CursorTrace
	IIEnemy* LastActor;
	IIEnemy* ThisActor;
};
