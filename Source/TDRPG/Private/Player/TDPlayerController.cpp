#include "Player/TDPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Interface/IEnemy.h"

ATDPlayerController::ATDPlayerController()
{
	bReplicates = true;
}

void ATDPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();
}

void ATDPlayerController::CursorTrace() // 마우스 커서 Trace하여 가리킨 적에 외곽선 효과
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (false == CursorHit.bBlockingHit) return;

	LastActor = ThisActor;
	ThisActor = Cast<IIEnemy>(CursorHit.GetActor());

	/**
	 * Line trace from cursor. There are several scenarios:
	 *  A. LastActor is null && ThisActor is null
	 *		- Do nothing
	 *	B. LastActor is null && ThisActor is valid
	 *		- Highlight ThisActor
	 *	C. LastActor is valid && ThisActor is null
	 *		- UnHighlight LastActor
	 *	D. Both actors are valid, but LastActor != ThisActor
	 *		- UnHighlight LastActor, and Highlight ThisActor
	 *	E. Both actors are valid, and are the same actor
	 *		- Do nothing
	 */

	if (LastActor == nullptr)
	{
		if (ThisActor != nullptr)
		{
			// Case B
			ThisActor->HighlightActor();
		}
		else
		{
			// Case A - both are null, do nothing
		}
	}
	else // LastActor is valid
	{
		if (ThisActor == nullptr)
		{
			// Case C
			LastActor->UnHighlightActor();
		}
		else // both actors are valid
		{
			if (LastActor != ThisActor)
			{
				// Case D
				LastActor->UnHighlightActor();
				ThisActor->HighlightActor();
			}
			else
			{
				// Case E - do nothing
			}
		}
	}
}

void ATDPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(RPGContext);

	 UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem)
	{
		Subsystem->AddMappingContext(RPGContext, 0);
	}


	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock); // Viewport에 마우스 락X
	InputModeData.SetHideCursorDuringCapture(false); // Cursor 숨기지 않기
	SetInputMode(InputModeData);
}

void ATDPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATDPlayerController::Move);
}

void ATDPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X); // Front
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y); // Right

	TWeakObjectPtr<APawn> ControlledPawn = GetPawn<APawn>();
	if (ControlledPawn.IsValid())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y); // Back
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X); // Left
	}
}
