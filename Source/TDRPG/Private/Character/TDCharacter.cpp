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
#include "NiagaraComponent.h"
#include "Component/TDInventoryComponent.h"
#include "Net/UnrealNetwork.h"

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
	CameraSpringArm->SetUsingAbsoluteRotation(true); // 캐릭터 회전 시 카메라 회전이 되는것이 아닌 Absolute Rotation
	CameraSpringArm->bDoCollisionTest = false;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraSpringArm);
	FollowCamera->bUsePawnControlRotation = false;

	// Niagara
	Niagara_LevelUp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("LevelUpEffect"));
	Niagara_LevelUp->SetupAttachment(RootComponent);
	Niagara_LevelUp->bAutoActivate = false;

	// Inventory Component
	TDInventoryComponent = CreateDefaultSubobject<UTDInventoryComponent>(TEXT("InventoryComponent"));
}

void ATDCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
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
//	checkf(TDPlayerState, TEXT("No TDPlayerState. Check: ATDCharacter::GetPlayerLevelBP_Implementation()"));
//	return TDPlayerState->GetPlayerLevel();
//}

int32 ATDCharacter::GetPlayerLevel()
{
	checkf(TDPlayerState, TEXT("No TDPlayerState. Check: ATDCharacter::GetPlayerLevel()"));
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
	checkf(TDPlayerState, TEXT("No TDPlayerState. Check: ATDCharacter::AddToExpCPP()"));
	TDPlayerState->AddToExp(InExp);
}

void ATDCharacter::AddToPlayerLevel(int32 InPlayerLevel)
{
	checkf(TDPlayerState, TEXT("No TDPlayerState. Check: ATDCharacter::AddToPlayerLevel()"));
	TDPlayerState->AddToPlayerLevel(InPlayerLevel);

	if (UTDAbilitySystemComponent* TDASC = Cast<UTDAbilitySystemComponent>(GetAbilitySystemComponent()))
	{
		TDASC->UpdateAbilityStatuses(TDPlayerState->GetPlayerLevel());
	}
}

void ATDCharacter::AddToPlayerLevelBP_Implementation(int32 InPlayerLevel)
{
	checkf(TDPlayerState, TEXT("No TDPlayerState. Check: ATDCharacter::AddToPlayerLevelBP_Implementation()"));
	TDPlayerState->AddToPlayerLevel(InPlayerLevel);

	if (UTDAbilitySystemComponent* TDASC = Cast<UTDAbilitySystemComponent>(GetAbilitySystemComponent()))
	{
		TDASC->UpdateAbilityStatuses(TDPlayerState->GetPlayerLevel());
	}
}

void ATDCharacter::AddToAttributePoints(int32 InAttributePoints)
{
	checkf(TDPlayerState, TEXT("No TDPlayerState. Check: ATDCharacter::AddToAttributePoints()"));
	TDPlayerState->AddToAttributePoints(InAttributePoints);
}

void ATDCharacter::AddToAttributePointsBP_Implementation(int32 InAttributePoints)
{
	checkf(TDPlayerState, TEXT("No TDPlayerState. Check: ATDCharacter::AddToAttributePointsBP_Implementation()"));
	TDPlayerState->AddToAttributePoints(InAttributePoints);
}

void ATDCharacter::AddToSkillPoints(int32 InSkillPoints)
{
	checkf(TDPlayerState, TEXT("No TDPlayerState. Check: ATDCharacter::AddToSkillPoints()"));
	TDPlayerState->AddToSkillPoints(InSkillPoints);
}

void ATDCharacter::AddToSkillPointsBP_Implementation(int32 InSpellPoints)
{
	checkf(TDPlayerState, TEXT("No TDPlayerState. Check: ATDCharacter::AddToSkillPointsBP_Implementation()"));
	TDPlayerState->AddToSkillPoints(InSpellPoints);
}

int32 ATDCharacter::GetExp() const
{
	checkf(TDPlayerState, TEXT("No TDPlayerState. Check: ATDCharacter::GetExp()"));
	return TDPlayerState->GetExp();
}

int32 ATDCharacter::GetExpBP_Implementation() const
{
	checkf(TDPlayerState, TEXT("No TDPlayerState. Check: ATDCharacter::GetExpBP_Implementation()"));
	return TDPlayerState->GetExp();
}

int32 ATDCharacter::FindLevelForExp(int32 InExp) const
{
	checkf(TDPlayerState, TEXT("No TDPlayerState. Check: ATDCharacter::FindLevelForExp()"));
	return TDPlayerState->TDDA_LevelUpInfo->FindDA_LevelUpForExp(InExp);
}

int32 ATDCharacter::FindLevelForExpBP_Implementation(int32 InExp) const
{
	checkf(TDPlayerState, TEXT("No TDPlayerState. Check: ATDCharacter::FindLevelForExpBP_Implementation()"));
	return TDPlayerState->TDDA_LevelUpInfo->FindDA_LevelUpForExp(InExp);
}

int32 ATDCharacter::GetAttributePointsReward(int32 PlayerLevel) const
{
	checkf(TDPlayerState, TEXT("No TDPlayerState. Check: ATDCharacter::GetAttributePointsReward()"));
	return TDPlayerState->TDDA_LevelUpInfo->DA_LevelUpInfo[PlayerLevel].AttributePointAward;
}

int32 ATDCharacter::GetAttributePointsRewardBP_Implementation(int32 PlayerLevel) const
{
	checkf(TDPlayerState, TEXT("No TDPlayerState. Check: ATDCharacter::GetAttributePointsRewardBP_Implementation()"));
	return TDPlayerState->TDDA_LevelUpInfo->DA_LevelUpInfo[PlayerLevel].AttributePointAward;
}

int32 ATDCharacter::GetSkillPointsReward(int32 PlayerLevel) const
{
	checkf(TDPlayerState, TEXT("No TDPlayerState. Check: ATDCharacter::GetSkillPointsReward()"));
	return TDPlayerState->TDDA_LevelUpInfo->DA_LevelUpInfo[PlayerLevel].SkillPointAward;
}

int32 ATDCharacter::GetSkillPointsRewardBP_Implementation(int32 PlayerLevel) const
{
	checkf(TDPlayerState, TEXT("No TDPlayerState. Check: ATDCharacter::GetSkillPointsRewardBP_Implementation()"));
	return TDPlayerState->TDDA_LevelUpInfo->DA_LevelUpInfo[PlayerLevel].SkillPointAward;
}

int32 ATDCharacter::GetAttributePoints() const
{
	checkf(TDPlayerState, TEXT("No TDPlayerState. Check: ATDCharacter::GetAttributePoints()"));
	return TDPlayerState->GetAttributePoints();
}

int32 ATDCharacter::GetAttributePointsBP_Implementation() const
{
	checkf(TDPlayerState, TEXT("No TDPlayerState. Check: ATDCharacter::GetAttributePointsBP_Implementation()"));
	return TDPlayerState->GetAttributePoints();
}

int32 ATDCharacter::GetSkillPoints() const
{
	checkf(TDPlayerState, TEXT("No TDPlayerState. Check: ATDCharacter::GetSkillPoints()"));
	return TDPlayerState->GetSkillPoints();
}

int32 ATDCharacter::GetSkillPointsBP_Implementation() const
{
	checkf(TDPlayerState, TEXT("No TDPlayerState. Check: ATDCharacter::GetSkillPointsBP_Implementation()"));
	return TDPlayerState->GetSkillPoints();
}

//void ATDCharacter::LevelUp_Implementation()
//{
//	Multicast_LevelUpParticleEffect();  // 서버 + 모든 클라이언트들에게 레벨업 이펙트를 보여줌.
//}

void ATDCharacter::LevelUpCPP()
{
	Multicast_LevelUpParticleEffect(); // 서버 + 모든 클라이언트들에게 레벨업 이펙트를 보여줌.
}

void ATDCharacter::InitAbilityActorInfo() // Ability actor 정보 초기화. Server와 Client 모두에서 콜
{
	if (false == IsValid(TDPlayerState))
	{
		TDPlayerState = GetPlayerState<ATDPlayerState>();
	}
	checkf(TDPlayerState, TEXT("No TDPlayerState. Check: ATDCharacter::InitAbilityActorInfo()")); // PlayerState 없을시 종료

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

void ATDCharacter::Multicast_LevelUpParticleEffect_Implementation() const
{
	check(Niagara_LevelUp);

	const FVector FollowCameraLocation = FollowCamera->GetComponentLocation();
	const FVector NiagaraLocation = Niagara_LevelUp->GetComponentLocation();
	const FRotator ToCameraRotation = (FollowCameraLocation - NiagaraLocation).Rotation();

	Niagara_LevelUp->SetWorldRotation(ToCameraRotation);
	Niagara_LevelUp->Activate(true);
}
