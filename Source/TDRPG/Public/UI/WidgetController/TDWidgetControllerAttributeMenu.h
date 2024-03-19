#pragma once
#include "CoreMinimal.h"
#include "UI/WidgetController/TDWidgetController.h"
#include "TDWidgetControllerAttributeMenu.generated.h"

/**
 * 
 */
UCLASS()
class TDRPG_API UTDWidgetControllerAttributeMenu : public UTDWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;
};
