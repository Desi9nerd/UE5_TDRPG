#include "UI/WidgetController/TDOverlayWidgetController.h"
#include "GAS/TDAttributeSet.h"

void UTDOverlayWidgetController::BroadcastInitialValues()
{
	const UTDAttributeSet* TDAttributeSet = CastChecked<UTDAttributeSet>(AttributeSet);

	OnHealthChanged.Broadcast(TDAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(TDAttributeSet->GetMaxHealth());
}
