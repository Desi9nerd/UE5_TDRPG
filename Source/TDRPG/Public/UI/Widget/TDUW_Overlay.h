#pragma once
#include "CoreMinimal.h"
#include "UI/Widget/TDUW.h"
#include "TDUW_Overlay.generated.h"

struct FUIWidgetRow;

/**
 * 
 */

UCLASS()
class TDRPG_API UTDUW_Overlay : public UTDUW
{
	GENERATED_BODY()

public:
	virtual void SetWidgetController(UObject* InWidgetController) override;


private:
	void HandleMessageWidget(FUIWidgetRow InUIWidgetRow);
};
