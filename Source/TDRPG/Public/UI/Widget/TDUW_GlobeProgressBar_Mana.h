#pragma once
#include "CoreMinimal.h"
#include "UI/Widget/TDUW_GlobeProgressBar.h"
#include "TDUW_GlobeProgressBar_Mana.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnManaChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMaxManaChanged);

/**
 * 
 */
UCLASS()
class TDRPG_API UTDUW_GlobeProgressBar_Mana : public UTDUW_GlobeProgressBar
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	FOnManaChanged OnManaChanged;
	FOnMaxManaChanged OnMaxManaChanged;

private:
	UFUNCTION()
	void BindingDelegates();

	UFUNCTION()
	void OnManaChanged_Event(float InValue);
	UFUNCTION()
	void OnMaxManaChanged_Event(float InValue);

	float Mana;
	float MaxMana;
};
