#include "UI/Widget/TDUW_GlobeProgressBar_Health.h"
#include "AbilitySystemComponent.h"
#include "GAS/TDAttributeSet.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/TDPlayerState.h"
#include "UI/WidgetController/TDWidgetControllerOverlay.h"

void UTDUW_GlobeProgressBar_Health::NativeConstruct()
{
	Super::NativeConstruct();
	
	// Deferred Binding 
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ThisClass::BindingDelegates, 1.f, false);
}

void UTDUW_GlobeProgressBar_Health::BindingDelegates()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	ATDPlayerState* PS = PlayerController->GetPlayerState<ATDPlayerState>();
	UTDAttributeSet* TDAttributeSet = Cast<UTDAttributeSet>(PS->GetAttributeSet());

	Health = TDAttributeSet->GetHealth();
	MaxHealth = TDAttributeSet->GetMaxHealth();
	SetProgressBarPercent(UKismetMathLibrary::SafeDivide(Health, MaxHealth));

	GetWidgetControllerOverlay(GetWorld())->OnHealthChanged.AddDynamic(this, &ThisClass::OnHealthChanged_Event);
	GetWidgetControllerOverlay(GetWorld())->OnMaxHealthChanged.AddDynamic(this, &ThisClass::OnMaxHealthChanged_Event);
}

void UTDUW_GlobeProgressBar_Health::OnHealthChanged_Event(float InValue)
{
	Health = InValue;

	SetProgressBarPercent(UKismetMathLibrary::SafeDivide(Health, MaxHealth));
}

void UTDUW_GlobeProgressBar_Health::OnMaxHealthChanged_Event(float InValue)
{
	MaxHealth = InValue;

	SetProgressBarPercent(UKismetMathLibrary::SafeDivide(Health, MaxHealth));
}
