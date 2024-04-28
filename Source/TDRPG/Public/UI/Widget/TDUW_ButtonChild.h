#pragma once
#include "CoreMinimal.h"
#include "UI/Widget/TDUW_Button.h"
#include "TDUW_ButtonChild.generated.h"

/**
 * 
 */
UCLASS()
class TDRPG_API UTDUW_ButtonChild : public UTDUW_Button
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_Title;

protected:
	virtual void NativePreConstruct() override;


};
