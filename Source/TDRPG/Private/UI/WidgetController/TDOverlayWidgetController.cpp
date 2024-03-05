#include "UI/WidgetController/TDOverlayWidgetController.h"
#include "GAS/TDAttributeSet.h"

void UTDOverlayWidgetController::BroadcastInitialValues()
{
	const UTDAttributeSet* TDAttributeSet = CastChecked<UTDAttributeSet>(AttributeSet);

	OnHealthChanged.Broadcast(TDAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(TDAttributeSet->GetMaxHealth());
}

void UTDOverlayWidgetController::BindCallbacksToDependencies() // TDAttributeSet�� �����Ϳ� �ݹ��Լ� ���ε�
{
	const UTDAttributeSet* TDAttributeSet = CastChecked<UTDAttributeSet>(AttributeSet);

	//** Health, MaxHealth�� ����ɶ� ���� �Ʒ��Լ�(HealthChanged, MaxHealthChanged)�� callback��
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
