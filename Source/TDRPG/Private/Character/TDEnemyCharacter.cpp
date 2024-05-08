#include "Character/TDEnemyCharacter.h"
#include "AI/AIController/TDAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GAS/TDAbilitySystemComponent.h"
#include "GAS/TDAttributeSet.h"
#include "TDRPG/TDRPG.h" // CUSTOM_DEPTH_RED
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UI/Widget/TDUW.h"
#include "GAS/TDAbilitySystemBPLibrary.h"
#include "GameplayTags/TDGameplayTags.h"

ATDEnemyCharacter::ATDEnemyCharacter()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->bUseRVOAvoidance = true; // Enemy끼리 겹쳐서 잘 안 움직이는것 방지. Agents avoid obstacles by using velocity vectors within a specified radius.

	AbilitySystemComponent = CreateDefaultSubobject<UTDAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UTDAttributeSet>("AttributeSet");

	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());
}

void ATDEnemyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (false == HasAuthority()) return;

	TDAIController = Cast<ATDAIController>(NewController);
	TDAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset); // BlackboardComponent 초기화.
	TDAIController->RunBehaviorTree(BehaviorTree); // BehaviorTree 실행

	//** Blackboard의 key를 찾고 해당 key의 value값을 설정하기.
	TDAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), false);
	TDAIController->GetBlackboardComponent()->SetValueAsBool(FName("RangedAttacker"), CharacterClass != ECharacterClass::Warrior);
}

void ATDEnemyCharacter::HighlightActor()
{
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	Weapon->SetRenderCustomDepth(true);
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

void ATDEnemyCharacter::UnHighlightActor()
{
	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);
}

void ATDEnemyCharacter::SetCombatTarget_Implementation(AActor* InCombatTarget)
{
	CombatTarget = InCombatTarget;
}

AActor* ATDEnemyCharacter::GetCombatTarget_Implementation() const
{
	return CombatTarget;
}

AActor* ATDEnemyCharacter::GetCombatTargetCPP() const
{
	return CombatTarget;
}

void ATDEnemyCharacter::Die(const FVector& RagdollImpulse)
{
	SetLifeSpan(LifeSpanUntilDead); // 5초 뒤 캐릭터 destroy

	if (IsValid(TDAIController))
	{	// Blackboard의 key를 찾고 해당 key의 value값을 설정
		TDAIController->GetBlackboardComponent()->SetValueAsBool(FName("Dead"), true);
	}

	Super::Die(RagdollImpulse);
}

int32 ATDEnemyCharacter::GetPlayerLevel()
{
	return Level;
}

void ATDEnemyCharacter::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bHitReacting = NewCount > 0;

	// 최대 이동속도: 피격중에는 0, 피격중이 아니라면 BaseWalkSpeed
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : BaseWalkSpeed;

	if (TDAIController && TDAIController->GetBlackboardComponent())
	{
		TDAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), bHitReacting);		
	}
}

void ATDEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	InitAbilityActorInfo();

	if (HasAuthority()) // Server
	{
		UTDAbilitySystemBPLibrary::GiveStartupAbilities(this, AbilitySystemComponent, CharacterClass); // GameplayAbility 초기값. ASC랑 캐릭터직업 넘김.
	}

	UTDUW* TDUserWidget = Cast<UTDUW>(HealthBar->GetUserWidgetObject());
	if (IsValid(TDUserWidget))
	{
		TDUserWidget->SetWidgetController(this);
	}

	//** Delegate Bind: TDWidgetControllerOverlay의 FOnAttributeChangeData 중 OnHealthChanged, OnMaxHealthChanged
	//** GameplayTagEvent 등록: Effect_HitReact 태그가 새로 생기거나 없어지면(=NewOrRemoved) HitReactTagChanged 콜백함수 호출
	const UTDAttributeSet* TDAttributeSet = Cast<UTDAttributeSet>(AttributeSet);
	if (IsValid(TDAttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(TDAttributeSet->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			}
		);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(TDAttributeSet->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
		);
		AbilitySystemComponent->RegisterGameplayTagEvent(FTDGameplayTags::GetTDGameplayTags().Effect_HitReact, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &ThisClass::HitReactTagChanged);

		OnHealthChanged.Broadcast(TDAttributeSet->GetHealth());
		OnMaxHealthChanged.Broadcast(TDAttributeSet->GetMaxHealth());
	}
}

void ATDEnemyCharacter::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UTDAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();

	if (HasAuthority()) // Server
	{
		InitializeDefaultAttributes();		
	}

	OnASCRegisteredDelegate.Broadcast(AbilitySystemComponent);
}

void ATDEnemyCharacter::InitializeDefaultAttributes() const
{
	UTDAbilitySystemBPLibrary::InitializeDefaultAttributes(this, CharacterClass, Level, AbilitySystemComponent);
}
