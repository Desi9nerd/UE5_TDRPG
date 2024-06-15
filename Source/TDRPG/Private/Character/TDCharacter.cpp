#include "Character/TDCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameMode/TDGameModeBase.h"
#include "GameMode/TDGameModeBase_Single.h"
#include "SaveGame/TDSaveGame_Load.h"
#include "AbilitySystemComponent.h"
#include "GAS/TDAbilitySystemComponent.h"
#include "Player/TDPlayerController.h"
#include "Player/TDPlayerState.h"
#include "UI/HUD/TDHUD.h"
#include "MotionWarpingComponent.h"
#include "GAS/Data/TDDA_LevelUp.h"
#include "NiagaraComponent.h"
#include "Component/TDDebuffComponent.h"
#include "Component/TDInventoryComponent.h"
#include "Component/TDZoomComponent.h"
#include "GameplayTags/TDGameplayTags.h"
#include "GAS/TDAbilitySystemBPLibrary.h"
#include "GAS/TDAttributeSet.h"
#include "GAS/Data/TDDA_Ability.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

ATDCharacter::ATDCharacter()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
	BaseWalkSpeed = 600.f;

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

	// Zoom
	ZoomComponent = CreateDefaultSubobject<UTDZoomComponent>(TEXT("ZoomComponent"));

	// Niagara
	Niagara_LevelUp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("LevelUpEffect"));
	Niagara_LevelUp->SetupAttachment(RootComponent);
	Niagara_LevelUp->bAutoActivate = false;

	// Inventory Component
	TDInventoryComponent = CreateDefaultSubobject<UTDInventoryComponent>(TEXT("InventoryComponent"));
	TDInventoryComponent->SetIsReplicated(true);


	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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

	if (GetTDGameModeBase_Single()) // Single Player
	{
		LoadProgress(); // 게임 SaveData 확인 후 로드하여 데이터 초기화.
		GetTDGameModeBase_Single()->LoadWorldState(GetWorld());
	}
	else // Multiplyer
	{
		InitializeDefaultAttributes(); // Attribute 초기화.
		AddCharacterAbilities(); // Abilities 초기화.
	}
}

void ATDCharacter::LoadProgress()
{
	UTDSaveGame_Load* SaveData = GetTDGameModeBase_Single()->RetrieveInGameSaveData();
	if (SaveData == nullptr) return;

	//* SaveData가 처음인지 아닌지 기준으로 기본값/저장된값으로 초기화.
	if (SaveData->bFirstTimeLoadIn) // SaveData가 생성되고 처음으로 로드 되는것이라면
	{
		InitializeDefaultAttributes(); // Attribute 초기화.
		AddCharacterAbilities(); // Abilities 초기화.
	}
	else // 처음이 아니라면
	{
		if (GetTDASC()) // SaveData에서 Abilities 로드하기.
		{
			GetTDASC()->AddCharacterAbilitiesFromSaveData(SaveData);
		}

		if (GetTDPlayerState()) // PlayerState의 데이터를 저장된 게임의 데이터값으로 초기화.
		{
			GetTDPlayerState()->SetPlayerLevel(SaveData->PlayerLevel);
			GetTDPlayerState()->SetExp(SaveData->Exp);
			GetTDPlayerState()->SetAttributePoints(SaveData->AttributePoints);
			GetTDPlayerState()->SetSkillPoints(SaveData->SkillPoints);
		}

		UTDAbilitySystemBPLibrary::InitializeDefaultAttributesFromSaveData(this, AbilitySystemComponent, SaveData);
	}
}

void ATDCharacter::OnRep_PlayerState() // 클라이언트
{
	Super::OnRep_PlayerState();

	// 클라이언트에 Init Ability actor info
	InitAbilityActorInfo(); 
}

TObjectPtr<UTDInventoryComponent> ATDCharacter::GetInventoryComponent()
{
	checkf(TDInventoryComponent, TEXT("No TDInventoryComponent. Check: ATDCharacter::GetInventoryComponent()"));
	return TDInventoryComponent;
}

TObjectPtr<ATDPlayerController> ATDCharacter::GetTDPlayerController()
{
	if (IsValid(TDPlayerController)) return TDPlayerController;

	TDPlayerController = Cast<ATDPlayerController>(GetController());
	return TDPlayerController;
}

TObjectPtr<ATDGameModeBase> ATDCharacter::GetTDGameModeBase()
{
	if (IsValid(TDGameModeBase)) return TDGameModeBase;

	TDGameModeBase = Cast<ATDGameModeBase>(UGameplayStatics::GetGameMode(this));
	return TDGameModeBase;
}

TObjectPtr<ATDGameModeBase_Single> ATDCharacter::GetTDGameModeBase_Single()
{
	if (IsValid(TDGameModeBase_Single)) return TDGameModeBase_Single;

	TDGameModeBase_Single = Cast<ATDGameModeBase_Single>(UGameplayStatics::GetGameMode(this));
	if (false == IsValid(TDGameModeBase_Single))
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to get TDGameModeBase_Single"));
	}
	return TDGameModeBase_Single;
}

TObjectPtr<ATDPlayerState> ATDCharacter::GetTDPlayerState()
{
	if (IsValid(TDPlayerState)) return TDPlayerState;

	TDPlayerState = GetPlayerState<ATDPlayerState>();
	return TDPlayerState;
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

void ATDCharacter::Die(const FVector& RagdollImpulse)
{
	Super::Die(RagdollImpulse);

	FTimerDelegate DeathTimerDelegate;
	// TODO: 멀티버젼도 생각해보기.
	DeathTimerDelegate.BindLambda([this]()
		{
			if (GetTDGameModeBase_Single())
			{
				GetTDGameModeBase_Single()->PlayerDeath(this); // GameMode에 플레이어 죽음
			}
		});

	GetWorldTimerManager().SetTimer(DeathTimer, DeathTimerDelegate, DeathTime, false);

	FollowCamera->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform); // 죽으면 카메라가 떨어지도록 Detach.
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

	if (GetTDASC())
	{
		GetTDASC()->UpdateAbilityStatuses(TDPlayerState->GetPlayerLevel());
	}
}

void ATDCharacter::AddToPlayerLevelBP_Implementation(int32 InPlayerLevel)
{
	checkf(TDPlayerState, TEXT("No TDPlayerState. Check: ATDCharacter::AddToPlayerLevelBP_Implementation()"));
	TDPlayerState->AddToPlayerLevel(InPlayerLevel);

	if (GetTDASC())
	{
		GetTDASC()->UpdateAbilityStatuses(TDPlayerState->GetPlayerLevel());
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

void ATDCharacter::ShowDecalBP_Implementation()
{
	if (GetTDPlayerController())
	{
		GetTDPlayerController()->ShowDecal();
		GetTDPlayerController()->bShowMouseCursor = false;
	}
}

void ATDCharacter::ShowDecal()
{
	if (GetTDPlayerController())
	{
		GetTDPlayerController()->ShowDecal();
		GetTDPlayerController()->bShowMouseCursor = false;
	}
}

void ATDCharacter::HideDecalBP_Implementation()
{
	if (GetTDPlayerController())
	{
		GetTDPlayerController()->HideDecal();
		GetTDPlayerController()->bShowMouseCursor = true;
	}
}

void ATDCharacter::HideDecal()
{
	if (GetTDPlayerController())
	{
		GetTDPlayerController()->HideDecal();
		GetTDPlayerController()->bShowMouseCursor = true;
	}
}

void ATDCharacter::SaveProgressBP_Implementation(const FName& CheckpointTag)
{
	UE_LOG(LogTemp, Warning, TEXT("BP Version is Called. Check ATDCharacter::SaveProgressBP_Implementation()"));
	/*
	if (GetTDGameModeBase_Single())
	{
		UTDSaveGame_Load* SaveData = GetTDGameModeBase_Single()->RetrieveInGameSaveData();
		if (false == IsValid(SaveData)) return;

		//* SaveData에 태그 기록.
		SaveData->PlayerStartTag = CheckpointTag;

		//* SaveData에 TDPlayerState 정보들(레벨, 경험치, 스탯 및 스킬 포인트) 기록.
		if (GetTDPlayerState())
		{
			SaveData->PlayerLevel = GetTDPlayerState()->GetPlayerLevel();
			SaveData->Exp = GetTDPlayerState()->GetExp();
			SaveData->AttributePoints = GetTDPlayerState()->GetAttributePoints();
			SaveData->SkillPoints = GetTDPlayerState()->GetSkillPoints();
		}
		//* SaveData에 기본스탯 기록.
		SaveData->Strength = UTDAttributeSet::GetStrengthAttribute().GetNumericValue(GetAttributeSet());
		SaveData->Intelligence = UTDAttributeSet::GetIntelligenceAttribute().GetNumericValue(GetAttributeSet());
		SaveData->Resilience = UTDAttributeSet::GetResilienceAttribute().GetNumericValue(GetAttributeSet());
		SaveData->Vigor = UTDAttributeSet::GetVigorAttribute().GetNumericValue(GetAttributeSet());

		//* 게임 저장.
		SaveData->bFirstTimeLoadIn = false; // 다음 번에 게임시작 시 처음 시작이 되지 않고 데이터를 불러오도록 false 설정.
		GetTDGameModeBase_Single()->SaveInGameProgressData(SaveData);
	}
	*/
}

void ATDCharacter::SaveProgress(const FName& CheckpointTag)
{
	if (GetTDGameModeBase_Single())
	{
		UTDSaveGame_Load* SaveData = GetTDGameModeBase_Single()->RetrieveInGameSaveData();
		if (false == IsValid(SaveData)) return;

		//* SaveData에 태그 기록.
		SaveData->PlayerStartTag = CheckpointTag;

		//* SaveData에 TDPlayerState 정보들(레벨, 경험치, 스탯 및 스킬 포인트) 기록.
		if (GetTDPlayerState())
		{
			SaveData->PlayerLevel = GetTDPlayerState()->GetPlayerLevel();
			SaveData->Exp = GetTDPlayerState()->GetExp();
			SaveData->AttributePoints = GetTDPlayerState()->GetAttributePoints();
			SaveData->SkillPoints = GetTDPlayerState()->GetSkillPoints();
		}
		//* SaveData에 기본스탯 기록.
		SaveData->Strength = UTDAttributeSet::GetStrengthAttribute().GetNumericValue(GetAttributeSet());
		SaveData->Intelligence = UTDAttributeSet::GetIntelligenceAttribute().GetNumericValue(GetAttributeSet());
		SaveData->Resilience = UTDAttributeSet::GetResilienceAttribute().GetNumericValue(GetAttributeSet());
		SaveData->Vigor = UTDAttributeSet::GetVigorAttribute().GetNumericValue(GetAttributeSet());

		SaveData->bFirstTimeLoadIn = false; // 다음 번에 게임시작 시 처음 시작이 되지 않고 데이터를 불러오도록 false 설정.


		if (false == HasAuthority()) return; // 서버에서만 저장. 서버가 아니면 return;

		FForEachAbility SaveAbilityDelegate;
		SaveData->SavedAbilities.Empty();

		//SaveAbilityDelegate.BindUFunction(this, &ThisClass::SaveGameSaveAbilities);
		SaveAbilityDelegate.BindLambda([this, SaveData](const FGameplayAbilitySpec& AbilitySpec)
			{
				const FGameplayTag AbilityTag = GetTDASC()->GetAbilityTagFromSpec(AbilitySpec);
				UTDDA_Ability* TDDA_Ability = UTDAbilitySystemBPLibrary::GetTDDA_Ability(this);
				FDA_Ability Info = TDDA_Ability->FindDA_AbilityForTag(AbilityTag);

				FSavedAbility SavedAbility;
				SavedAbility.GameplayAbility = Info.Ability;
				SavedAbility.AbilityLevel = AbilitySpec.Level;
				SavedAbility.AbilitySlot = GetTDASC()->GetSlotTagFromAbilityTag(AbilityTag);
				SavedAbility.AbilityStatus = GetTDASC()->GetStatusTagFromAbilityTag(AbilityTag);
				SavedAbility.AbilityTag = AbilityTag;
				SavedAbility.AbilityType = Info.AbilityType;

				SaveData->SavedAbilities.AddUnique(SavedAbility);

			});

		GetTDASC()->ForEachAbility(SaveAbilityDelegate);


		//* 게임 저장.
		GetTDGameModeBase_Single()->SaveInGameProgressData(SaveData);
	}
}

void ATDCharacter::SaveGameSaveAbilities(const FGameplayAbilitySpec& AbilitySpec)
{
	/*
	const FGameplayTag AbilityTag = TDASC->GetAbilityTagFromSpec(AbilitySpec);
	UTDDA_Ability* TDDA_Ability = UTDAbilitySystemBPLibrary::GetTDDA_Ability(this);
	FDA_Ability Info = TDDA_Ability->FindDA_AbilityForTag(AbilityTag);

	FSavedAbility SavedAbility;
	SavedAbility.GameplayAbility = Info.Ability;
	SavedAbility.AbilityLevel = AbilitySpec.Level;
	SavedAbility.AbilitySlot = TDASC->GetInputTagFromAbilityTag(AbilityTag);
	SavedAbility.AbilityStatus = TDASC->GetStatusTagFromAbilityTag(AbilityTag);
	SavedAbility.AbilityTag = AbilityTag;
	SavedAbility.AbilityType = Info.AbilityType;

	SaveData->SavedAbilities.AddUnique(SavedAbility);
	*/
}

void ATDCharacter::OnRep_Stunned()
{
	if (GetTDASC())
	{
		const FTDGameplayTags& GameplayTags = FTDGameplayTags::GetTDGameplayTags();
		FGameplayTagContainer BlockedTags;
		BlockedTags.AddTag(GameplayTags.BlockTag_CursorTrace);
		BlockedTags.AddTag(GameplayTags.BlockTag_InputHeld);
		BlockedTags.AddTag(GameplayTags.BlockTag_InputPressed);
		BlockedTags.AddTag(GameplayTags.BlockTag_InputReleased);
		if (bStunned)
		{
			GetTDASC()->AddLooseGameplayTags(BlockedTags);
			StunDebuffComponent->Activate(); // Stun디버프 활성화.
		}
		else
		{
			GetTDASC()->RemoveLooseGameplayTags(BlockedTags);
			StunDebuffComponent->Deactivate(); // Stun디버프 비활성화.
		}
	}
}

void ATDCharacter::OnRep_Burned()
{
	if (bBurned)
	{
		DotDamageDebuffComponent->Activate();
	}
	else
	{
		DotDamageDebuffComponent->Deactivate();
	}
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
	OnASCRegisteredDelegate.Broadcast(AbilitySystemComponent);
	AbilitySystemComponent->RegisterGameplayTagEvent(FTDGameplayTags::GetTDGameplayTags().Debuff_Stun, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &ThisClass::StunTagChanged);
	
	// 서버는 모든 PlayerController를 소유.
	// 하지만 각각의 클라이언트는 하나의 PlayerController만 소유하고 나머진 Proxy로 가지고 있다
	// 그렇기 때문에 Assert로 체크하면 안 된다.
	if (GetTDPlayerController())
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
