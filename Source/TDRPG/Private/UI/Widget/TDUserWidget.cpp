#include "UI/Widget/TDUserWidget.h"

void UTDUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet(); // Blueprint���� ����
}
