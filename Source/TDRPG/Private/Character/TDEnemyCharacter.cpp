#include "Character/TDEnemyCharacter.h"
#include "GAS/TDAbilitySystemComponent.h"
#include "GAS/TDAttributeSet.h"
#include "TDRPG/TDRPG.h" // CUSTOM_DEPTH_RED
#include "Components/WidgetComponent.h"
#include "UI/Widget/TDUserWidget.h"
#include "GAS/TDAbilitySystemBPLibrary.h"

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

int32 ATDEnemyCharacter::GetPlayerLevel()
{
	return Level;
}

void ATDEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	InitAbilityActorInfo();

	UTDUserWidget* TDUserWidget = Cast<UTDUserWidget>(HealthBar->GetUserWidgetObject());
	if (IsValid(TDUserWidget))
	{
		TDUserWidget->SetWidgetController(this);
	}

	//** Delegate Bind: TDWidgetControllerOverlay의 FOnAttributeChangeData 중 OnHealthChanged, OnMaxHealthChanged
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
