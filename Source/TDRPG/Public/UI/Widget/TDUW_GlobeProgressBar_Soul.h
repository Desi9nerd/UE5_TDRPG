#pragma once
#include "CoreMinimal.h"
#include "UI/Widget/TDUW_GlobeProgressBar.h"
#include "TDUW_GlobeProgressBar_Soul.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSoulChanged);

/**
 * 
 */
UCLASS()
class TDRPG_API UTDUW_GlobeProgressBar_Soul : public UTDUW_GlobeProgressBar
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	FOnSoulChanged OnSoulChanged;

private:
	UFUNCTION()
	void OnSoulChanged_Event(float InValue);

	float Soul;
	float MaxSoul;
};
