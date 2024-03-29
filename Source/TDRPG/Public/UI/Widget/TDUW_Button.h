#pragma once
#include "CoreMinimal.h"
#include "UI/Widget/TDUserWidget.h"
#include "TDUW_Button.generated.h"

class USizeBox;
class UTextBlock;
class UImage;
class UButton;
struct FSlateBrush;

/**
 * 
 */
UCLASS()
class TDRPG_API UTDUW_Button : public UTDUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	USizeBox* SizeBox_Root;
	UPROPERTY(meta = (BindWidget))
	UImage* Image_Border;
	UPROPERTY(meta = (BindWidget))
	UButton* Button;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_X;
	
protected:
	virtual void NativePreConstruct() override;

private:
	void SetAttributeSet();
	void UpdateBoxSize();
	void UpdateBorderBrush();
	void UpdateButtonBrushes();
	void UpdateText();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess), Category = "Button Properties")
	float BoxWidth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess), Category = "Button Properties")
	float BoxHeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess), Category = "Button Properties")
	FSlateBrush BorderBrush;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess), Category = "Button Properties")
	FSlateBrush ButtonNormalBrush;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess), Category = "Button Properties")
	FSlateBrush ButtonHoveredBrush;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess), Category = "Button Properties")
	FSlateBrush ButtonPressedBrush;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess), Category = "Button Properties")
	FSlateBrush ButtonDisabledBrush;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess), Category = "Button Properties")
	FText ButtonText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess), Category = "Button Properties")
	UObject* FontFamily;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess), Category = "Button Properties")
	int OutlineSize;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess), Category = "Button Properties")
	float FontSize;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess), Category = "Button Properties")
	int LetterSpacing;


};
