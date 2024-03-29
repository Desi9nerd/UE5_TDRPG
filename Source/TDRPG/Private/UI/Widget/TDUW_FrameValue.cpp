#include "UI/Widget/TDUW_FrameValue.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"

void UTDUW_FrameValue::NativePreConstruct()
{
	Super::NativePreConstruct();

	UpdateFrameSize();
	UpdateBackgroundBrush();
}

UTDUW_FrameValue::UTDUW_FrameValue(const FObjectInitializer& ObjectInitializer)
{
	//ensureMsgf(SizeBox_Root, TEXT("SizeBox_Root를 할당하십시오."));
	//ensureMsgf(Image_Background, TEXT("Image_Background를 할당하십시오."));
	//ensureMsgf(TextBlock_Value, TEXT("number를 할당하십시오."));
}

void UTDUW_FrameValue::UpdateFrameSize()
{
	SizeBox_Root->SetWidthOverride(BoxWidth);
	SizeBox_Root->SetHeightOverride(BoxHeight);
}

void UTDUW_FrameValue::UpdateBackgroundBrush()
{
	Image_Background->SetBrush(BackgroundBrush);
}
