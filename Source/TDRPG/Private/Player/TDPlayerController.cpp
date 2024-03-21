#include "Player/TDPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInput/TDEnhancedInputComponent.h"
#include "Interface/IEnemy.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GAS/TDAbilitySystemComponent.h"
#include "GameplayTags/TDGameplayTags.h"
#include "Components/SplineComponent.h"

ATDPlayerController::ATDPlayerController()
{
	bReplicates = true;

	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
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
	
	const TWeakObjectPtr<UTDEnhancedInputComponent> TDEnhancedInputComponent = CastChecked<UTDEnhancedInputComponent>(InputComponent);
	TDEnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATDPlayerController::Move);
	TDEnhancedInputComponent->BindAbilityActions(InputData, this, &ATDPlayerController::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}

void ATDPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (InputTag.MatchesTagExact(FTDGameplayTags::GetTDGameplayTags().InputTag_RMB)) // 마우스 우클릭
	{
		bTargeting = ThisActor ? true : false;
		bAutoRunning = false;
	}
}

void ATDPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (false == IsValid(GetASC())) return;

	GetASC()->InputTagReleased(InputTag);
}

void ATDPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (false == InputTag.MatchesTagExact(FTDGameplayTags::GetTDGameplayTags().InputTag_RMB))
	{
		if (IsValid(GetASC()))
		{
			GetASC()->InputTagHeld(InputTag);
		}
		return;
	}

	if (bTargeting) // 타겟팅 O
	{
		if (IsValid(GetASC()))
		{
			GetASC()->InputTagHeld(InputTag);
		}
	}
	else // 타겟팅 X
	{
		FollowTime += GetWorld()->GetDeltaSeconds();

		FHitResult Hit;
		if (GetHitResultUnderCursor(ECC_Visibility, false, Hit)) // 마우스 커서가 스크린(=월드)에 충돌된 지점이 있다면 true
		{
			CachedDestination = Hit.ImpactPoint; // Hit된 위치를 도착지점으로 설정
		}

		const TWeakObjectPtr<APawn> ControlledPawn = GetPawn(); // 여기서 폰은 플레이어
		if (ControlledPawn.IsValid())
		{
			// 방향 = (도착지점 - Pawn의 위치)를 Normalize한 벡터
			const FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
			ControlledPawn->AddMovementInput(WorldDirection); // 해당 방향으로 움직임
		}
	}
}

TObjectPtr<UTDAbilitySystemComponent> ATDPlayerController::GetASC()
{
	if (false == IsValid(TDASC))
	{
		TDASC = Cast<UTDAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}

	return TDASC;
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
