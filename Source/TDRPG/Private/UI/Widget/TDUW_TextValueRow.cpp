#include "UI/Widget/TDUW_TextValueRow.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "UI/Widget/TDUW_FrameValue.h"
#include "UI/WidgetController/TDWidgetControllerAttributeMenu.h"

void UTDUW_TextValueRow::NativePreConstruct()
{
	Super::NativePreConstruct();

	UpdateFrameSize();
}

void UTDUW_TextValueRow::NativeConstruct()
{
	Super::NativeConstruct();

	//GetAttributeMenuWidgetController(this);
}

UTDUW_TextValueRow::UTDUW_TextValueRow()
{
	BoxWidth = 750.f;
	BoxHeight = 60.f;
}

void UTDUW_TextValueRow::UpdateFrameSize()
{
	SizeBox_Root->SetWidthOverride(BoxWidth);
	SizeBox_Root->SetHeightOverride(BoxHeight);
}

void UTDUW_TextValueRow::SetLabelText(FText InLabelText)
{
	TextBlock_Label->SetText(InLabelText);
}

void UTDUW_TextValueRow::SetNumericalValueInt(int32 NewValue)
{
	FramedValue->TextBlock_Value->SetText(FText::FromString(FString::FromInt(NewValue)));
}
