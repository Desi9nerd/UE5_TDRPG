#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TDPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class IIEnemy;

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
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> RPGContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	void Move(const FInputActionValue& InputActionValue);

	//** ���콺�� ����Ű�°��� Enemy�� ������ �ܰ��� Highlight
	void CursorTrace(); // ���콺 CursorTrace
	IIEnemy* LastActor;
	IIEnemy* ThisActor;
};
