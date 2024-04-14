#include "Character/TDCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "AbilitySystemComponent.h"
#include "GAS/TDAbilitySystemComponent.h"
#include "Player/TDPlayerController.h"
#include "Player/TDPlayerState.h"
#include "UI/HUD/TDHUD.h"
#include "MotionWarpingComponent.h"
#include "GAS/Data/TDDA_LevelUp.h"

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

void ATDCharacter::PossessedBy(AController* NewController) // 서버
{
	Super::PossessedBy(NewController);

	// 서버에 Init Ability actor info 
	InitAbilityActorInfo(); 
	AddCharacterAbilities();
}

void ATDCharacter::OnRep_PlayerState() // 클라이언트
{
	Super::OnRep_PlayerState();

	// 클라이언트에 Init Ability actor info
	InitAbilityActorInfo(); 
}

//int32 ATDCharacter::GetPlayerLevelBP_Implementation()
//{
//	if (false == IsValid(TDPlayerState))
//	{
//		TDPlayerState = GetPlayerState<ATDPlayerState>();
//	}
//	check(TDPlayerState);
//
//	return TDPlayerState->GetPlayerLevel();
//}

int32 ATDCharacter::GetPlayerLevel()
{
	check(TDPlayerState);

	return TDPlayerState->GetPlayerLevel();
}

//void ATDCharacter::AddToExp_Implementation(int32 InExp)
//{
//	check(TDPlayerState);
//
//	TDPlayerState->AddToExp(InExp);
//}

void ATDCharacter::AddToExpCPP(int32 InExp)
{
	check(TDPlayerState);

	TDPlayerState->AddToExp(InExp);
}

void ATDCharacter::AddToPlayerLevel(int32 InPlayerLevel)
{
	check(TDPlayerState);

	TDPlayerState->AddToPlayerLevel(InPlayerLevel);
}

void ATDCharacter::AddToPlayerLevelBP_Implementation(int32 InPlayerLevel)
{
	check(TDPlayerState);

	TDPlayerState->AddToPlayerLevel(InPlayerLevel);
}

void ATDCharacter::AddToAttributePoints(int32 InAttributePoints)
{
	check(TDPlayerState);

	// TODO: TDPlayerState->AddToAttributePoints(InPlayerLevel);
}

void ATDCharacter::AddToAttributePointsBP_Implementation(int32 InAttributePoints)
{
	check(TDPlayerState);

	// TODO: TDPlayerState->AddToAttributePoints(InPlayerLevel);
}

void ATDCharacter::AddToSkillPoints(int32 InSkillPoints)
{
	check(TDPlayerState);

	// TODO: TDPlayerState->AddToSkillPoints(InPlayerLevel);
}

void ATDCharacter::AddToSkillPointsBP_Implementation(int32 InSpellPoints)
{
	check(TDPlayerState);

	// TODO: TDPlayerState->AddToSkillPoints(InPlayerLevel);
}

int32 ATDCharacter::GetExp() const
{
	check(TDPlayerState);

	return TDPlayerState->GetExp();
}

int32 ATDCharacter::GetExpBP_Implementation() const
{
	check(TDPlayerState);

	return TDPlayerState->GetExp();
}

int32 ATDCharacter::FindLevelForExp(int32 InExp) const
{
	check(TDPlayerState);
	
	return TDPlayerState->TDDA_LevelUpInfo->FindDA_LevelUpForExp(InExp);
}

int32 ATDCharacter::FindLevelForExpBP_Implementation(int32 InExp) const
{
	check(TDPlayerState);

	return TDPlayerState->TDDA_LevelUpInfo->FindDA_LevelUpForExp(InExp);
}

int32 ATDCharacter::GetAttributePointsReward(int32 PlayerLevel) const
{
	check(TDPlayerState);

	return TDPlayerState->TDDA_LevelUpInfo->DA_LevelUpInfo[PlayerLevel].AttributePointAward;
}

int32 ATDCharacter::GetAttributePointsRewardBP_Implementation(int32 PlayerLevel) const
{
	check(TDPlayerState);

	return TDPlayerState->TDDA_LevelUpInfo->DA_LevelUpInfo[PlayerLevel].AttributePointAward;
}

int32 ATDCharacter::GetSkillPointsReward(int32 PlayerLevel) const
{
	check(TDPlayerState);

	return TDPlayerState->TDDA_LevelUpInfo->DA_LevelUpInfo[PlayerLevel].SkillPointAward;
}

int32 ATDCharacter::GetSkillPointsRewardBP_Implementation(int32 PlayerLevel) const
{
	check(TDPlayerState);

	return TDPlayerState->TDDA_LevelUpInfo->DA_LevelUpInfo[PlayerLevel].SkillPointAward;
}

//void ATDCharacter::LevelUp_Implementation()
//{
//	
//}

void ATDCharacter::LevelUpCPP()
{
	
}

void ATDCharacter::InitAbilityActorInfo() // Ability actor 정보 초기화. Server와 Client 모두에서 콜
{
	if (false == IsValid(TDPlayerState))
	{
		TDPlayerState = GetPlayerState<ATDPlayerState>();
	}
	check(TDPlayerState); // 예외처리. PlayerState 없을시 종료

	TDPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(TDPlayerState, this);
	Cast<UTDAbilitySystemComponent>(TDPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet(); // AbilityActorInfo 설정하기

	AbilitySystemComponent = TDPlayerState->GetAbilitySystemComponent();
	AttributeSet = TDPlayerState->GetAttributeSet();

	InitializeDefaultAttributes(); // Attibutes 초기값 설정하기

	// 서버는 모든 PlayerController를 소유.
	// 하지만 각각의 클라이언트는 하나의 PlayerController만 소유하고 나머진 Proxy로 가지고 있다
	// 그렇기 때문에 Assert로 체크하면 안 된다.
	if (ATDPlayerController* TDPlayerController = Cast<ATDPlayerController>(GetController()))
	{
		if (ATDHUD* TDHUD = Cast<ATDHUD>(TDPlayerController->GetHUD()))
		{
			TDHUD->InitOverlay(TDPlayerController, TDPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}
}
