#include "UI/Widget/TDUW_SkillIcon_EquippedRowButton.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "UI/WidgetController/TDWidgetControllerSkillMenu.h"

void UTDUW_SkillIcon_EquippedRowButton::NativePreConstruct()
{
	UpdateBoxSize(SizeBox_Root, BoxWidth, BoxHeight);
	Image_Background->SetBrush(TransparentBrush);
	Image_SkillIcon->SetBrush(TransparentBrush);
	Image_Ring->SetBrush(TransparentBrush);
	Text_Cooldown->SetRenderOpacity(0.f);
	SetPadding(Image_Background, Padding_Background, Padding_Background, Padding_Background, Padding_Background);
	SetPadding(Image_SkillIcon, Padding_SkillIcon, Padding_SkillIcon, Padding_SkillIcon, Padding_SkillIcon);
}

void UTDUW_SkillIcon_EquippedRowButton::NativeConstruct()
{
	Super::NativeConstruct();

	Button_ClickHover->OnClicked.AddDynamic(this, &ThisClass::OnButtonClicked);
}

void UTDUW_SkillIcon_EquippedRowButton::NativeDestruct()
{
	Super::NativeDestruct();

	if (IsValid(TDWidgetControllerSkillMenu))
	{
		TDWidgetControllerSkillMenu->DA_AbilityInfoDelegate.Clear();
	}
}

void UTDUW_SkillIcon_EquippedRowButton::OnButtonClicked()
{
	if (IsValid(TDWidgetControllerSkillMenu))
	{
		TDWidgetControllerSkillMenu->SkillRowGlobePressed(TDInputTag, TDAbilityTypeTag);
	}
}
