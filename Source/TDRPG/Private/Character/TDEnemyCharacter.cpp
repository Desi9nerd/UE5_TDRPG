#include "Character/TDEnemyCharacter.h"
#include "Actor/TDItemActor.h"
#include "AI/AIController/TDAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GAS/TDAbilitySystemComponent.h"
#include "GAS/TDAttributeSet.h"
#include "TDRPG/TDRPG.h" // CUSTOM_DEPTH_RED
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameMode/TDGameModeBase.h"
#include "UI/Widget/TDUW.h"
#include "GAS/TDAbilitySystemBPLibrary.h"
#include "GameplayTags/TDGameplayTags.h"
#include "GAS/Data/TDDA_DropItem.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

ATDEnemyCharacter::ATDEnemyCharacter()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->bUseRVOAvoidance = true; // Enemy끼리 겹쳐서 잘 안 움직이는것 방지. Agents avoid obstacles by using velocity vectors within a specified radius.
	BaseWalkSpeed = 300.f;

	AbilitySystemComponent = CreateDefaultSubobject<UTDAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UTDAttributeSet>(TEXT("AttributeSet"));

	HealthBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
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

	// ASC에 태그 델리게이트 등록:  Debuff_Stun 태그 감지 시 StunTagChanged 함수가 실행되도록 한다.
	AbilitySystemComponent->RegisterGameplayTagEvent(FTDGameplayTags::GetTDGameplayTags().Debuff_Stun, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &ThisClass::StunTagChanged);
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

	SpawnItemsAfterDeath(); // 몬스터 사망 후 아이템 드롭

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
	AbilitySystemComponent->RegisterGameplayTagEvent(FTDGameplayTags::GetTDGameplayTags().Debuff_Stun, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &ThisClass::StunTagChanged);

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

void ATDEnemyCharacter::StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	Super::StunTagChanged(CallbackTag, NewCount);

	if (TDAIController && TDAIController->GetBlackboardComponent())
	{
		TDAIController->GetBlackboardComponent()->SetValueAsBool(FName(TEXT("Stunned")), bStunned);
	}
}

void ATDEnemyCharacter::SpawnItemsAfterDeath()
{
	const ATDGameModeBase* TDGameMode = Cast<ATDGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (false == IsValid(TDGameMode)) return;

	LootItems = TDGameMode->TDDADropItem->GetLootItems();
	int32 NumOfItems = LootItems.Num();
	LootRotations = EvenlySpacedRotators(GetActorForwardVector(), GetActorUpVector(), 360.f, NumOfItems); // 아이템 여러개를 일정 간격으로 스폰하기 위해 TArray<FRotator> 계산.

	SpawnLoopCnt = 0;

	GetWorld()->GetTimerManager().SetTimer(LootTimer, this, &ThisClass::SpawnLootItem, 0.1f, true);
}

void ATDEnemyCharacter::SpawnLootItem()
{
	if (SpawnLoopCnt < LootItems.Num())
	{
		FVector Location = GetActorLocation() - FVector(0.f, 0.f, 50.f) + UKismetMathLibrary::GetForwardVector(LootRotations[SpawnLoopCnt]) * UKismetMathLibrary::RandomFloatInRange(MinSpawnDistance, MaxSpawnDistance);
		FTransform SpawnTransform = UKismetMathLibrary::MakeTransform(Location, LootRotations[SpawnLoopCnt]);

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		ATDItemActor* ItemActor = GetWorld()->SpawnActor<ATDItemActor>(LootItems[SpawnLoopCnt].LootClass, SpawnTransform, SpawnParams);
		if (IsValid(ItemActor))
		{

		}

		SpawnLoopCnt++;
	}
	else
	{
		LootTimer.Invalidate();
	}
}

TArray<FRotator> ATDEnemyCharacter::EvenlySpacedRotators(const FVector& ForwardVector, const FVector& Axis, float Spread, int32 NumOfRotators)
{
	TArray<FRotator> Rotators;

	if (NumOfRotators > 1)
	{
		const float DeltaSpread = Spread / (NumOfRotators - 1);
		for (int32 i = 0; i < NumOfRotators; i++)
		{
			const FVector LeftOfSpread = ForwardVector.RotateAngleAxis(-Spread / 2.f, Axis);
			const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
			Rotators.Add(Direction.Rotation());
		}
	}
	else
	{
		Rotators.Add(ForwardVector.Rotation());
	}

	return Rotators;
}
