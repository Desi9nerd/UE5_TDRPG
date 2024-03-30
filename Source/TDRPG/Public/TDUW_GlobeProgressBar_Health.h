#pragma once
#include "CoreMinimal.h"
#include "UI/Widget/TDUW_GlobeProgressBar.h"
#include "TDUW_GlobeProgressBar_Health.generated.h"

DECLARE_DELEGATE_OneParam(FOnHealthChanged, const float&);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHealthChanged);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMaxHealthChanged);

/**
 * 
 */
UCLASS()
class TDRPG_API UTDUW_GlobeProgressBar_Health : public UTDUW_GlobeProgressBar
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	FOnHealthChanged OnHealthChanged;
	//FOnMaxHealthChanged OnMaxHealthChanged;

private:
	UFUNCTION()
	void OnHealthChanged_Event(float InValue);
	UFUNCTION()
	void OnMaxHealthChanged_Event(float InValue);

	float Health;
	float MaxHealth;
};
