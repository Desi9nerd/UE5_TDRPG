#include "UI/Widget/TDUW_GlobeProgressBar_Mana.h"
#include "GAS/TDAttributeSet.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/TDPlayerState.h"
#include "UI/WidgetController/TDWidgetControllerOverlay.h"

void UTDUW_GlobeProgressBar_Mana::NativeConstruct()
{
	Super::NativeConstruct();

	// Deferred Binding 
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ThisClass::BindingDelegates, 1.f, false);
}

void UTDUW_GlobeProgressBar_Mana::BindingDelegates()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	ATDPlayerState* PS = PlayerController->GetPlayerState<ATDPlayerState>();
	UTDAttributeSet* TDAttributeSet = Cast<UTDAttributeSet>(PS->GetAttributeSet());

	Mana = TDAttributeSet->GetMana();
	MaxMana = TDAttributeSet->GetMaxMana();
	SetProgressBarPercent(UKismetMathLibrary::SafeDivide(Mana, MaxMana));

	GetWidgetControllerOverlay(GetWorld())->OnManaChanged.AddDynamic(this, &ThisClass::OnManaChanged_Event);
	GetWidgetControllerOverlay(GetWorld())->OnMaxManaChanged.AddDynamic(this, &ThisClass::OnMaxManaChanged_Event);
}

void UTDUW_GlobeProgressBar_Mana::OnManaChanged_Event(float InValue)
{
	Mana = InValue;

	SetProgressBarPercent(UKismetMathLibrary::SafeDivide(Mana, MaxMana));
}


void UTDUW_GlobeProgressBar_Mana::OnMaxManaChanged_Event(float InValue)
{
	MaxMana = InValue;

	SetProgressBarPercent(UKismetMathLibrary::SafeDivide(Mana, MaxMana));
}
