#include "Character/TDEnemyCharacter.h"
#include "GAS/TDAbilitySystemComponent.h"
#include "GAS/TDAttributeSet.h"
#include "TDRPG/TDRPG.h" // CUSTOM_DEPTH_RED
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UI/Widget/TDUserWidget.h"
#include "GAS/TDAbilitySystemBPLibrary.h"
#include "GameplayTags/TDGameplayTags.h"

ATDEnemyCharacter::ATDEnemyCharacter()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	AbilitySystemComponent = CreateDefaultSubobject<UTDAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UTDAttributeSet>("AttributeSet");

	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());
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

void ATDEnemyCharacter::Die()
{
	SetLifeSpan(LifeSpanUntilDead); // 5초 뒤 캐릭터 destroy

	Super::Die();
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
}

void ATDEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	InitAbilityActorInfo();
	UTDAbilitySystemBPLibrary::GiveStartupAbilities(this, AbilitySystemComponent); // GameplayAbility 초기값

	UTDUserWidget* TDUserWidget = Cast<UTDUserWidget>(HealthBar->GetUserWidgetObject());
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

	InitializeDefaultAttributes();
}

void ATDEnemyCharacter::InitializeDefaultAttributes() const
{
	UTDAbilitySystemBPLibrary::InitializeDefaultAttributes(this, CharacterClass, Level, AbilitySystemComponent);
}
