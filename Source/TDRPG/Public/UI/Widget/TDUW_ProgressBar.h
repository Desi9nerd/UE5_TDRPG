#pragma once
#include "CoreMinimal.h"
#include "UI/Widget/TDUserWidget.h"
#include "TDUW_ProgressBar.generated.h"

class USizeBox;
class UOverlay;
class UProgressBar;
struct FSlateBrush;

/**
 * 
 */
UCLASS()
class TDRPG_API UTDUW_ProgressBar : public UTDUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	USizeBox* SizeBox_Root;
	UPROPERTY(meta = (BindWidget))
	UOverlay* Overlay_Root;
	UPROPERTY(meta = (BindWidget))
	UProgressBar* ProgressBar_Ghost;
	UPROPERTY(meta = (BindWidget))
	UProgressBar* ProgressBar_Front;

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION(BlueprintCallable)
	virtual void SetBarPercent(const float& InValue, const float& InMaxValue);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess), Category = "Progress Bar Properties")
	float BoxWidth = 90.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess), Category = "Progress Bar Properties")
	float BoxHeight = 7.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess), Category = "Progress Bar Properties")
	FSlateBrush FrontBarFillBrush;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess), Category = "Progress Bar Properties")
	FSlateBrush GhostBarFillBrush;

private:
	void UpdateBoxSize();
	void UpdateFrontFillBrush();
	void UpdateGhostFillBrush();
	void SetBarVisibility(bool bVisible);
	void InterpGhostBar(float InDeltaTime);
	void UpdateGhostInterpTarget(const float& InTarget);
	
	void HideProgressBar(const float& InTarget);
	void DelayGhostPercentTarget(const float& InTarget);

	bool bProgressBarVisibility;

	float GhostPercentTarget = 1.f;

	FTimerHandle HideTimer;
};
