#pragma once
#include "CoreMinimal.h"
#include "UI/Widget/TDUserWidget.h"
#include "TDUW_FrameValue.generated.h"

class UTextBlock;
class USizeBox;
class UImage;
/**
 * 
 */
UCLASS()
class TDRPG_API UTDUW_FrameValue : public UTDUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativePreConstruct() override;
	UTDUW_FrameValue(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(meta = (BindWidget))
	USizeBox* SizeBox_Root;
	UPROPERTY(meta = (BindWidget))
	UImage* Image_Background;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextBlock_Value;

private:
	void UpdateFrameSize();
	void UpdateBackgroundBrush();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess), Category = "Frame Properties")
	float BoxWidth = 80.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess), Category = "Frame Properties")
	float BoxHeight = 45.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess), Category = "Frame Properties")
	FSlateBrush BackgroundBrush;
};
