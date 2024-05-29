#include "UI/Widget/TDUW_GlobeProgressBar_Soul.h"
#include "Kismet/KismetMathLibrary.h"
#include "UI/WidgetController/TDWidgetControllerOverlay.h"

void UTDUW_GlobeProgressBar_Soul::NativeConstruct()
{
	Super::NativeConstruct();

	//TDWidgetControllerOverlay = GetWidgetControllerOverlay(GetWorld());
	//check(TDWidgetControllerOverlay);
	//
	//TDWidgetControllerOverlay->OnSoulChanged.AddDynamic(this, &ThisClass::OnSoulChanged_Event);
}

void UTDUW_GlobeProgressBar_Soul::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UTDUW_GlobeProgressBar_Soul::OnSoulChanged_Event(float InValue)
{
	Soul = InValue;

	SetProgressBarPercent(UKismetMathLibrary::SafeDivide(Soul, 100.f));
}
