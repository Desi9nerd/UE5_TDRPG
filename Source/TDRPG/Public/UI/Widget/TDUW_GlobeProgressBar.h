#pragma once
#include "CoreMinimal.h"
#include "UI/Widget/TDUserWidget.h"
#include "TDUW_GlobeProgressBar.generated.h"

class USizeBox;
class UOverlay;
class UProgressBar;
class UImage;
struct FSlateBrush;

/**
 * 
 */
UCLASS()
class TDRPG_API UTDUW_GlobeProgressBar : public UTDUserWidget
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
	UImage* Image_Background;
	UPROPERTY(meta = (BindWidget))
	UProgressBar* ProgressBar_Globe;
	UPROPERTY(meta = (BindWidget))
	UImage* Image_Glass;

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION(BlueprintCallable)
	virtual void SetProgressBarPercent(float InPercent);

	void GlobePercentSet(float InPercent);
	void SetNewGhostPercentTarget(float InPercent);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess), Category = "Globe ProgressBar Properties")
	float BoxWidth = 250.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess), Category = "Globe ProgressBar Properties")
	float BoxHeight = 250.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess), Category = "Globe ProgressBar Properties")
	FSlateBrush BackgroundBrush;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess), Category = "Globe ProgressBar Properties")
	FSlateBrush ProgressBarBrush;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess), Category = "Globe ProgressBar Properties")
	float GlobePadding;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess), Category = "Globe ProgressBar Properties")
	FSlateBrush GlassBrush;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess), Category = "Ghost Globe Properties")
	float GhostPercentTarget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess), Category = "Ghost Globe Properties")
	float GhostDelay;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess), Category = "Ghost Globe Properties")
	FSlateBrush GhostProgressBarBrush;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess), Category = "Ghost Globe Properties")
	float GhostInterpSpeed;

	bool bGlobeInitialized = false;

private:
	void UpdateBoxSize();
	void UpdateBackgrondBrush();
	void UpdateGlobeBrush();
	void UpdateGlobePadding();
	void UpdateGlassBrush();
	void UpdateGlassPadding();
	void UpdateGhostGlobeBrush();

	void SetGhostProgressBarPercent(const float& InPercent);
};
