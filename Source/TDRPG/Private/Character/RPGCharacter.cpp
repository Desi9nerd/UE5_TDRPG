#include "Character/RPGCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "AbilitySystemComponent.h"
#include "Player/RPGPlayerState.h"

ARPGCharacter::ARPGCharacter()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	CameraSpringArm->TargetArmLength = 750.0f;
	CameraSpringArm->bUsePawnControlRotation = false;
	CameraSpringArm->bEnableCameraLag = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraSpringArm);
	FollowCamera->bUsePawnControlRotation = false;
}

void ARPGCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitAbilityActorInfo(); // Init Ability actor info for the Server
}

void ARPGCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	InitAbilityActorInfo(); // Init Ability actor info for the Client
}

void ARPGCharacter::InitAbilityActorInfo() // Ability actor 정보 초기화
{
	TObjectPtr<ARPGPlayerState> RPGPlayerState = GetPlayerState<ARPGPlayerState>();
	check(RPGPlayerState); // 예외처리. PlayerState 없을시 종료

	RPGPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(RPGPlayerState, this);
	AbilitySystemComponent = RPGPlayerState->GetAbilitySystemComponent();
	AttributeSet = RPGPlayerState->GetAttributeSet();
}