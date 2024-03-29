#include "UI/Widget/TDUW_Button.h"
#include "Components/SizeBox.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UTDUW_Button::NativePreConstruct()
{
	Super::NativePreConstruct();
	
	UpdateBoxSize();
	UpdateBorderBrush();
	UpdateButtonBrushes();
	UpdateText();
}

void UTDUW_Button::UpdateBoxSize()
{
	SizeBox_Root->SetWidthOverride(BoxWidth);
	SizeBox_Root->SetHeightOverride(BoxHeight);
}

void UTDUW_Button::UpdateBorderBrush()
{
	Image_Border->SetBrush(BorderBrush);
}

void UTDUW_Button::UpdateButtonBrushes()
{
	FButtonStyle ButtonStyle;
	ButtonStyle.Normal = ButtonNormalBrush;
	ButtonStyle.Hovered = ButtonHoveredBrush;
	ButtonStyle.Pressed = ButtonPressedBrush;
	ButtonStyle.Disabled = ButtonDisabledBrush;

	Button->SetStyle(ButtonStyle);
}

void UTDUW_Button::UpdateText()
{
	Text_X->SetText(ButtonText);

	FSlateFontInfo FontInfo;
	FontInfo.FontObject = FontFamily;
	FontInfo.OutlineSettings = FFontOutlineSettings(OutlineSize);
	FontInfo.Size = FontSize;
	FontInfo.LetterSpacing = LetterSpacing;

	Text_X->SetFont(FontInfo);
}
