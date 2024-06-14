#pragma once
#include "CoreMinimal.h"
#include "UI/Widget/TDUW.h"
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
class TDRPG_API UTDUW_Button : public UTDUW
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	USizeBox* SizeBox_Root;
	UPROPERTY(meta = (BindWidget))
	UImage* Image_Border;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* Button;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* Text_X;
	
protected:
	virtual void NativePreConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess), Category = "Button Properties")
	float BoxWidth = 40.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess), Category = "Button Properties")
	float BoxHeight = 40.f;
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

private:
	void SetAttributeSet();
	void UpdateBoxSize();
	void UpdateBorderBrush();
	void UpdateButtonBrushes();
	void UpdateText();
};
