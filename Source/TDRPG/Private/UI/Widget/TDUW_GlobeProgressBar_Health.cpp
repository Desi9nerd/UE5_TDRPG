#include "UI/Widget/TDUW_GlobeProgressBar_Health.h"
#include "Kismet/KismetMathLibrary.h"
#include "UI/WidgetController/TDWidgetControllerOverlay.h"

void UTDUW_GlobeProgressBar_Health::NativeConstruct()
{
	Super::NativeConstruct();

	//TDWidgetControllerOverlay = GetWidgetControllerOverlay(GetWorld());
	//check(TDWidgetControllerOverlay);

	GetWidgetControllerOverlay(GetWorld())->OnHealthChanged.AddDynamic(this, &ThisClass::OnHealthChanged_Event);
	GetWidgetControllerOverlay(GetWorld())->OnMaxHealthChanged.AddDynamic(this, &ThisClass::OnMaxHealthChanged_Event);

}

void UTDUW_GlobeProgressBar_Health::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
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
