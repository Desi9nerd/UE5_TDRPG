#include "UI/Widget/TDUW_GlobeProgressBar_Mana.h"
#include "Kismet/KismetMathLibrary.h"
#include "UI/WidgetController/TDWidgetControllerOverlay.h"

void UTDUW_GlobeProgressBar_Mana::NativeConstruct()
{
	Super::NativeConstruct();

	TDWidgetControllerOverlay = GetWidgetControllerOverlay(GetWorld());
	check(TDWidgetControllerOverlay);

	TDWidgetControllerOverlay->OnManaChanged.AddDynamic(this, &ThisClass::OnManaChanged_Event);
	TDWidgetControllerOverlay->OnMaxManaChanged.AddDynamic(this, &ThisClass::OnMaxManaChanged_Event);
}

void UTDUW_GlobeProgressBar_Mana::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
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
