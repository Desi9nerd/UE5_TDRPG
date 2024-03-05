#include "Character/TDCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "AbilitySystemComponent.h"
#include "Player/TDPlayerState.h"

ATDCharacter::ATDCharacter()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	
	// Camera
	CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	CameraSpringArm->SetupAttachment(RootComponent);
	CameraSpringArm->SetRelativeLocationAndRotation(FVector(0.f, 0.f, 0.f), FRotator(-45.f, 0.f, 0.f));
	CameraSpringArm->TargetArmLength = 750.0f;
	CameraSpringArm->bUsePawnControlRotation = false;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraSpringArm);
	FollowCamera->bUsePawnControlRotation = false;

}

void ATDCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitAbilityActorInfo(); // Init Ability actor info for the Server
}

void ATDCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	InitAbilityActorInfo(); // Init Ability actor info for the Client
}

void ATDCharacter::InitAbilityActorInfo() // Ability actor 정보 초기화
{
	TObjectPtr<ATDPlayerState> TDPlayerState = GetPlayerState<ATDPlayerState>();
	check(TDPlayerState); // 예외처리. PlayerState 없을시 종료

	TDPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(TDPlayerState, this);
	AbilitySystemComponent = TDPlayerState->GetAbilitySystemComponent();
	AttributeSet = TDPlayerState->GetAttributeSet();
}