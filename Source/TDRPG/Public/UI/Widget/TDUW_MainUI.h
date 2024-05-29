#pragma once
#include "CoreMinimal.h"
#include "TDUW_Overlay.h"
#include "TDUW_MainUI.generated.h"

/**
 * 
 */
UCLASS()
class TDRPG_API UTDUW_MainUI : public UTDUW_Overlay
{
	GENERATED_BODY()

public:
	void NativePreConstruct() override;

private:
	void SetSkillIconInputTag();
};
