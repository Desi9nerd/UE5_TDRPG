#include "Player/TDPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInput/TDEnhancedInputComponent.h"
#include "Interface/IEnemy.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GAS/TDAbilitySystemComponent.h"
#include "GameplayTags/TDGameplayTags.h"
#include "Components/SplineComponent.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "GameFramework/Character.h"
#include "UI/WidgetComponent/TDWidgetComponent.h"

ATDPlayerController::ATDPlayerController()
{
	bReplicates = true;

	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
}

void ATDPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();
	AutoRun();
}

void ATDPlayerController::ShowDamageNumber_Implementation(float DamageAmount, ACharacter* TargetCharacter, bool bBlockedHit, bool bCriticalHit)
{
	check(DamageTextComponentClass);

	if (IsLocalController()) // Client�� ��� ������ �ؽ�Ʈ O. Server�� ��� X
	{
		if (IsValid(TargetCharacter) && DamageTextComponentClass)
		{
			//** TDWidgetComponent ���� ����. TargetCharacter���� DamageText����
			UTDWidgetComponent* DamageText = NewObject<UTDWidgetComponent>(TargetCharacter, DamageTextComponentClass);
			DamageText->RegisterComponent(); // �����ڰ� �ƴѰ����� ���� �����ϱ� ������ �ʿ�. 
			DamageText->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform); // ĳ���� ��ġ�� ����
			DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform); // ĳ���Ϳ� ��� DamageText �ִϸ��̼� ȿ���� ���ϴµ��� ���̵��� ����
			DamageText->SetDamageText(DamageAmount, bBlockedHit, bCriticalHit);
		}
	}
}

void ATDPlayerController::CursorTrace() // ���콺 Ŀ�� Trace
{
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (false == CursorHit.bBlockingHit) return;

	// ���콺�� ����Ų Enemy �ܰ��� ȿ��
	LastActor = ThisActor;
	ThisActor = Cast<IIEnemy>(CursorHit.GetActor());

	if (LastActor != ThisActor)
	{
		if (LastActor) LastActor->UnHighlightActor();
		if (ThisActor) ThisActor->HighlightActor();
	}
}

void ATDPlayerController::BeginPlay()
{
	Super::BeginPlay();

	PlayerControlledPawn = GetPawn(); // ���⼭ ���� �÷��̾�

	check(RPGContext);

	 UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem)
	{
		Subsystem->AddMappingContext(RPGContext, 0);
	}


	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock); // Viewport�� ���콺 ��X
	InputModeData.SetHideCursorDuringCapture(false); // Cursor ������ �ʱ�
	SetInputMode(InputModeData);
}

void ATDPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	const TWeakObjectPtr<UTDEnhancedInputComponent> TDEnhancedInputComponent = CastChecked<UTDEnhancedInputComponent>(InputComponent);
	TDEnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATDPlayerController::Move);
	TDEnhancedInputComponent->BindAction(ShiftAction, ETriggerEvent::Started, this, &ATDPlayerController::ShiftPressed);
	TDEnhancedInputComponent->BindAction(ShiftAction, ETriggerEvent::Completed, this, &ATDPlayerController::ShiftReleased);
	TDEnhancedInputComponent->BindAbilityActions(InputData, this, &ATDPlayerController::AbilityInputTagPressed, &ATDPlayerController::AbilityInputTagReleased, &ATDPlayerController::AbilityInputTagHeld);
}

void ATDPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (InputTag.MatchesTagExact(FTDGameplayTags::GetTDGameplayTags().InputTag_RMB)) // ���콺 ��Ŭ��
	{
		bTargeting = ThisActor ? true : false;
		bAutoRunning = false;
	}
}

void ATDPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (false == InputTag.MatchesTagExact(FTDGameplayTags::GetTDGameplayTags().InputTag_RMB))
	{
		if (GetASC())
		{
			GetASC()->InputTagReleased(InputTag);
		}
		return;
	}

	if (bTargeting) // Ÿ���� O
	{
		if (GetASC())
		{
			GetASC()->InputTagReleased(InputTag);
		}
	}
	else // Ÿ���� X
	{
		if (FollowTime <= ShortPressThreshold && IsValid(PlayerControlledPawn))
		{
			const TWeakObjectPtr<UNavigationPath> NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, PlayerControlledPawn->GetActorLocation(), CachedDestination);
			if (NavPath.IsValid())
			{
				Spline->ClearSplinePoints();
				for (const FVector& NavPointLocation : NavPath->PathPoints) 
				{
					// NavPath�� ����Ʈ���� Spline�� ����Ʈ�� �߰�
					Spline->AddSplinePoint(NavPointLocation, ESplineCoordinateSpace::World);
					DrawDebugSphere(GetWorld(), NavPointLocation, 10.f, 8, FColor::Green, false, 5.f);
				}
				CachedDestination = NavPath->PathPoints[NavPath->PathPoints.Num() - 1]; // CachedDestination�� ������ PathPoints�� ����. �̷����ϸ� AutoRunning�� �׻� ���ް����� ���������� �ȴ�.
				bAutoRunning = true;
			}
		}
		FollowTime = 0.f;
		bTargeting = false;
	}
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

	if (bTargeting) // Ÿ���� O
	{
		if (IsValid(GetASC()))
		{
			GetASC()->InputTagHeld(InputTag);
		}
	}
	else // Ÿ���� X
	{
		FollowTime += GetWorld()->GetDeltaSeconds();
		
		if (CursorHit.bBlockingHit) // ���콺 Ŀ���� ��ũ��(=����)�� �浹�� ������ �ִٸ� true
		{
			CachedDestination = CursorHit.ImpactPoint; // Hit�� ��ġ�� ������������ ����
		}
		
		if (IsValid(PlayerControlledPawn))
		{
			// ���� = (�������� - Pawn�� ��ġ)�� Normalize�� ����
			const FVector WorldDirection = (CachedDestination - PlayerControlledPawn->GetActorLocation()).GetSafeNormal();
			PlayerControlledPawn->AddMovementInput(WorldDirection); // �ش� �������� ������
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

void ATDPlayerController::AutoRun()
{
	if (false == bAutoRunning) return;
	
	if (IsValid(PlayerControlledPawn))
	{
		// Pawn�� �׻� Spline �ٷ� ���� ���� �����Ƿ� Pawn�� ���� ����� Spline ��ġ�� LocationOnSpline���� ����
		const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(PlayerControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
		const FVector Direction = Spline->FindDirectionClosestToWorldLocation(LocationOnSpline, ESplineCoordinateSpace::World); // LocationOnSpline��ġ������ Spline ����
		PlayerControlledPawn->AddMovementInput(Direction);

		const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length(); // �̵��ؾ��� �Ÿ�
		if (DistanceToDestination <= AutoRunAcceptanceRadius) // �̵��ؾ��� �Ÿ��� �ڵ��̵� ���� �������� �۴ٸ�
		{
			bAutoRunning = false;
		}
	}
}

void ATDPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X); // Front
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y); // Right
	
	if (IsValid(PlayerControlledPawn))
	{
		PlayerControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y); // Back
		PlayerControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X); // Left
	}
}
