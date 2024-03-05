#include "UI/WidgetController/TDOverlayWidgetController.h"
#include "GAS/TDAttributeSet.h"

void UTDOverlayWidgetController::BroadcastInitialValues()
{
	const UTDAttributeSet* TDAttributeSet = CastChecked<UTDAttributeSet>(AttributeSet);

	OnHealthChanged.Broadcast(TDAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(TDAttributeSet->GetMaxHealth());
}

void UTDOverlayWidgetController::BindCallbacksToDependencies() // TDAttributeSet의 데이터와 콜백함수 바인딩
{
	const UTDAttributeSet* TDAttributeSet = CastChecked<UTDAttributeSet>(AttributeSet);

	//** Health, MaxHealth가 변경될때 마다 아래함수(HealthChanged, MaxHealthChanged)가 callback됨
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		TDAttributeSet->GetHealthAttribute()).AddUObject(this, &UTDOverlayWidgetController::HealthChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		TDAttributeSet->GetMaxHealthAttribute()).AddUObject(this, &UTDOverlayWidgetController::MaxHealthChanged);
}

void UTDOverlayWidgetController::HealthChanged(const FOnAttributeChangeData& Data) const
{
	OnHealthChanged.Broadcast(Data.NewValue);
}

void UTDOverlayWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxHealthChanged.Broadcast(Data.NewValue);
}
