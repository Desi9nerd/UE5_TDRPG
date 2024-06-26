﻿#include "Player/TDPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInput/TDEnhancedInputComponent.h"
#include "Interface/IEnemy.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GAS/TDAbilitySystemComponent.h"
#include "GameplayTags/TDGameplayTags.h"
#include "Components/SplineComponent.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Character/TDCharacter.h"
#include "Component/TDInventoryComponent.h"
#include "Actor/TDDecalActor.h"
#include "Component/TDZoomComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "TDRPG/TDRPG.h"
#include "UI/HUD/TDHUD.h"
#include "UI/WidgetComponent/TDWidgetComponent.h"
#include "UI/Widget/Inventory/TDUW_Inventory.h"
#include "UI/Widget/Inventory/TDUW_InventoryMenuUI.h"
#include "UI/Widget/Inventory/TDUW_InventoryPanel.h"

ATDPlayerController::ATDPlayerController()
{
	bReplicates = true;

	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
}

void ATDPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();
	AutoRun();
	UpdateDecalLocation();
}

ATDHUD* ATDPlayerController::GetTDHUD()
{
	if (IsValid(TDHUD)) return TDHUD;

	TDHUD = Cast<ATDHUD>(GetHUD());
	return TDHUD;
}

TObjectPtr<ATDCharacter> ATDPlayerController::GetTDCharacter()
{
	if (IsValid(TDCharacter)) return TDCharacter;

	TDCharacter = Cast<ATDCharacter>(GetPawn());
	return TDCharacter;
}

void ATDPlayerController::ShowDamageNumber_Implementation(float DamageAmount, ACharacter* TargetCharacter, bool bBlockedHit, bool bCriticalHit)
{
	check(DamageTextComponentClass);

	if (IsLocalController()) // Client인 경우 데미지 텍스트 O. Server인 경우 X
	{
		if (IsValid(TargetCharacter) && DamageTextComponentClass)
		{
			//** TDWidgetComponent 동적 생성. TargetCharacter에서 DamageText생성
			UTDWidgetComponent* DamageText = NewObject<UTDWidgetComponent>(TargetCharacter, DamageTextComponentClass);
			DamageText->RegisterComponent(); // 생성자가 아닌곳에서 동적 생성하기 때문에 필요. 
			DamageText->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform); // 캐릭터 위치에 부착
			DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform); // 캐릭터에 떼어서 DamageText 애니메이션 효과가 원하는데로 보이도록 적용
			DamageText->SetDamageText(DamageAmount, bBlockedHit, bCriticalHit);
		}
	}
}

void ATDPlayerController::CursorTrace() // 마우스 커서 Trace
{
	//**************************************************************************************
	//** 예외 처리: BlockTag_CursorTrace 상태
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FTDGameplayTags::GetTDGameplayTags().BlockTag_CursorTrace))
	{
		if (LastActor) LastActor->UnHighlightActor();
		if (ThisActor) ThisActor->UnHighlightActor();
		LastActor = nullptr;
		ThisActor = nullptr;
		return;
	}
	//**************************************************************************************

	// 캐릭터를 가리키는 경우, decal이 적용안되도록 ECC를 설정.
	const ECollisionChannel ECC_TraceChannel = IsValid(Decal) ? ECC_ExcludingCharacters : ECC_Visibility;
	GetHitResultUnderCursor(ECC_TraceChannel, false, CursorHit);

	if (false == CursorHit.bBlockingHit) return;

	// 마우스로 가리킨 Enemy 외곽선 효과
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

	PlayerControlledPawn = GetPawn(); // 여기서 폰은 플레이어

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
	TDEnhancedInputComponent->BindAction(ZoomInputAction, ETriggerEvent::Triggered, this, &ATDPlayerController::OnMouseWheel);
	TDEnhancedInputComponent->BindAction(ShiftAction, ETriggerEvent::Started, this, &ATDPlayerController::ShiftPressed);
	TDEnhancedInputComponent->BindAction(ShiftAction, ETriggerEvent::Completed, this, &ATDPlayerController::ShiftReleased);
	TDEnhancedInputComponent->BindAbilityActions(InputData, this, &ATDPlayerController::AbilityInputTagPressed, &ATDPlayerController::AbilityInputTagReleased, &ATDPlayerController::AbilityInputTagHeld);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	TDEnhancedInputComponent->BindAction(OpenInventoryInputAction, ETriggerEvent::Triggered, this, &ATDPlayerController::OnOpenInventoryTriggered);
	TDEnhancedInputComponent->BindAction(PickUpItemInputAction, ETriggerEvent::Triggered, this, &ATDPlayerController::OnPickupItemTriggered);
}

void ATDPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	//**************************************************************************************
	//** 예외 처리: BlockTag_InputPressed 상태
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FTDGameplayTags::GetTDGameplayTags().BlockTag_InputPressed))
	{
		return;
	}
	//**************************************************************************************


	if (InputTag.MatchesTagExact(FTDGameplayTags::GetTDGameplayTags().InputTag_RMB)) // 마우스 우클릭
	{
		bTargeting = ThisActor ? true : false;
		bAutoRunning = false;
	}

	if (GetASC()) 
	{
		GetASC()->InputTagPressed(InputTag); // AbilitySystemComponent의 InputTag 눌림 함수를 콜.
	}
}

void ATDPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	//**************************************************************************************
	//** 예외 처리: BlockTag_InputReleased 상태
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FTDGameplayTags::GetTDGameplayTags().BlockTag_InputReleased))
	{
		return;
	}
	//**************************************************************************************

	if (false == InputTag.MatchesTagExact(FTDGameplayTags::GetTDGameplayTags().InputTag_RMB))
	{
		if (GetASC())
		{
			GetASC()->InputTagReleased(InputTag);
		}
		return;
	}

	if (bTargeting) // 타겟팅 O
	{
		if (GetASC())
		{
			GetASC()->InputTagReleased(InputTag);
		}
	}
	else // 타겟팅 X
	{
		if (FollowTime <= ShortPressThreshold && IsValid(PlayerControlledPawn))
		{
			const TWeakObjectPtr<UNavigationPath> NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, PlayerControlledPawn->GetActorLocation(), CachedDestination);
			if (NavPath.IsValid())
			{
				Spline->ClearSplinePoints();
				for (const FVector& NavPointLocation : NavPath->PathPoints) 
				{
					// NavPath의 포인트들을 Spline의 포인트로 추가
					Spline->AddSplinePoint(NavPointLocation, ESplineCoordinateSpace::World);
					//DrawDebugSphere(GetWorld(), NavPointLocation, 10.f, 8, FColor::Green, false, 5.f);
				}

				if (NavPath->PathPoints.Num() > 0)
				{
					CachedDestination = NavPath->PathPoints[NavPath->PathPoints.Num() - 1]; // CachedDestination를 마지막 PathPoints로 설정. 이렇게하면 AutoRunning때 항상 도달가능한 도착지점이 된다.
					bAutoRunning = true;					
				}

				// BlockTag_InputPressed 상태일 때는 마우스 클릭 효과가 안 나타나도록 하기.
				if (GetASC() && false == GetASC()->HasMatchingGameplayTag(FTDGameplayTags::GetTDGameplayTags().BlockTag_InputPressed))
				{
					UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ClickMoveEffect, CachedDestination); // 마우스 클릭 효과
				}
			}
		}
		FollowTime = 0.f;
		bTargeting = false;
	}
}

void ATDPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	//**************************************************************************************
	//** 예외 처리: BlockTag_InputHeld 상태
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FTDGameplayTags::GetTDGameplayTags().BlockTag_InputHeld))
	{
		return;
	}
	//**************************************************************************************

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
		
		if (CursorHit.bBlockingHit) // 마우스 커서가 스크린(=월드)에 충돌된 지점이 있다면 true
		{
			CachedDestination = CursorHit.ImpactPoint; // Hit된 위치를 도착지점으로 설정
		}
		
		if (IsValid(PlayerControlledPawn))
		{
			// 방향 = (도착지점 - Pawn의 위치)를 Normalize한 벡터
			const FVector WorldDirection = (CachedDestination - PlayerControlledPawn->GetActorLocation()).GetSafeNormal();
			PlayerControlledPawn->AddMovementInput(WorldDirection); // 해당 방향으로 움직임
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
		// Pawn은 항상 Spline 바로 위에 있지 않으므로 Pawn에 가장 가까운 Spline 위치를 LocationOnSpline으로 설정
		const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(PlayerControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
		const FVector Direction = Spline->FindDirectionClosestToWorldLocation(LocationOnSpline, ESplineCoordinateSpace::World); // LocationOnSpline위치에서의 Spline 방향
		PlayerControlledPawn->AddMovementInput(Direction);

		const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length(); // 이동해야할 거리
		if (DistanceToDestination <= AutoRunAcceptanceRadius) // 이동해야할 거리가 자동이동 설정 범위보다 작다면
		{
			bAutoRunning = false;
		}
	}
}

void ATDPlayerController::UpdateDecalLocation()
{
	if (IsValid(Decal))
	{
		Decal->SetActorLocation(CursorHit.ImpactPoint);
	}
}

void ATDPlayerController::ShowDecal()
{
	if (false == IsValid(Decal))
	{
		Decal = GetWorld()->SpawnActor<ATDDecalActor>(DecalClass);
	}
}

void ATDPlayerController::HideDecal()
{
	if (IsValid(Decal))
	{
		Decal->Destroy();
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

void ATDPlayerController::OnMouseWheel(const FInputActionValue& Value)
{
	float WheelValue = Value.Get<float>();

	GetTDCharacter()->GetTDZoomComponent()->SetZoomValue(WheelValue);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ATDPlayerController::OnOpenInventoryTriggered(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Open Inventory Button Pressed!!"));

	if (bInventoryIsOpen)
	{
		OpenCloseInventoryWidget(false);
	}
	else
	{
		OpenCloseInventoryWidget(true);		
	}
}

void ATDPlayerController::OnPickupItemTriggered(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Pickup Button Pressed!!"));
	
	if (GetTDCharacter())
	{
		GetTDCharacter()->GetInventoryComponent()->PickupItem();
	}
}

void ATDPlayerController::OpenCloseInventoryWidget(bool bOpen) // 인벤토리 열기/닫기.
{
	if (IsLocalController())
	{
		Client_OpenCloseInventoryWidget(bOpen);		
	}
}

void ATDPlayerController::Client_OpenCloseInventoryWidget_Implementation(bool bOpen)
{
	if (bOpen) // 열기
	{
		if (IsValid(GetTDHUD()->GetInventoryPanelWidget()))
		{
			GetTDHUD()->GetInventoryPanelWidget()->SetVisibility(ESlateVisibility::Visible);
			UGameplayStatics::PlaySound2D(this, Sound_OpenInventory);
			bInventoryIsOpen = true;
		}
	}
	else // 닫기
	{
		if (IsValid(GetTDHUD()->GetInventoryPanelWidget()))
		{
			GetTDHUD()->GetInventoryPanelWidget()->SetVisibility(ESlateVisibility::Hidden);
			UGameplayStatics::PlaySound2D(this, Sound_CloseInventory);
			bInventoryIsOpen = false;
		}
	}

	if (false == bTemp)
	{
		bTemp = true;
	}
}
