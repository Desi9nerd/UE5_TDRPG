#include "UI/Widget/TDUW_SkillIcon_Button.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "UI/WidgetController/TDWidgetControllerSkillMenu.h"

void UTDUW_SkillIcon_Button::NativePreConstruct()
{
	UpdateBoxSize(SizeBox_Root, BoxWidth, BoxHeight);
	Image_Background->SetBrush(BackgroundBrush);
	SetPadding(Image_Background, Padding_Background, Padding_Background, Padding_Background, Padding_Background);
	SetPadding(Image_SkillIcon, Padding_SkillIcon, Padding_SkillIcon, Padding_SkillIcon, Padding_SkillIcon);
}

void UTDUW_SkillIcon_Button::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(GetWidgetControllerSkillMenu(this)))
	{
		GetWidgetControllerSkillMenu(this)->DA_AbilityInfoDelegate.AddDynamic(this, &ThisClass::OnAbilityInfoDelegate_Event2);
		GetWidgetControllerSkillMenu(this)->SkillIconReassignedDelegate.AddDynamic(this, &ThisClass::OnSkillIconReassignedDelegate_Event);
	}

	Button_ClickHover->OnClicked.AddDynamic(this, &ThisClass::OnButtonClicked);
}

void UTDUW_SkillIcon_Button::NativeDestruct()
{
	Super::NativeDestruct();

	if(TDWidgetControllerSkillMenu)
	{
		// TODO : Unbind all Event
		TDWidgetControllerSkillMenu->DA_AbilityInfoDelegate.RemoveAll(this);
		TDWidgetControllerSkillMenu->SkillIconReassignedDelegate.RemoveAll(this);
	}
}

void UTDUW_SkillIcon_Button::OnAbilityInfoDelegate_Event2(const FDA_Ability& InDA_Ability)
{
	if (false == InDA_Ability.AbilityTag.MatchesTagExact(TDAbilityTag)) return; // 해당 스킬이 없다면 return;

	// Option 1: Locked
	if (InDA_Ability.StatusTag.MatchesTagExact(FTDGameplayTags::GetTDGameplayTags().Abilities_Status_Locked))
	{
		Image_Background->SetBrushFromMaterial(MI_Locked);
		Image_SkillIcon->SetBrushFromTexture(Texture_Locked);
	}
	else
	{
		// Option 2: Equipped or Unlocked
		if (InDA_Ability.StatusTag.MatchesTagExact(FTDGameplayTags::GetTDGameplayTags().Abilities_Status_Equipped) ||
			InDA_Ability.StatusTag.MatchesTagExact(FTDGameplayTags::GetTDGameplayTags().Abilities_Status_Unlocked))
		{
			Image_Background->SetBrushFromMaterial(MI_EquippedUnlocked);
			Image_SkillIcon->SetBrushFromTexture(Texture_EquippedUnlocked);
		}
		else
		{
			// Option 3: Eligible
			if (InDA_Ability.StatusTag.MatchesTagExact(FTDGameplayTags::GetTDGameplayTags().Abilities_Status_Eligible))
			{
				Image_Background->SetBrushFromMaterial(MI_Locked); // Background는 Lock
				Image_SkillIcon->SetBrushFromTexture(Texture_EquippedUnlocked); // SkillIcon은 EquippedUnlocked
			}
		}
	}
}

void UTDUW_SkillIcon_Button::OnSkillIconReassignedDelegate_Event(const FGameplayTag& InAbilityTag)
{
	if (false == InAbilityTag.MatchesTagExact(TDAbilityTag)) return; // 해당 스킬이 없다면 return;

	Image_Selection->SetRenderOpacity(0.f);
	PlaySound(Sound_Click);

	bSelected = false;
}

void UTDUW_SkillIcon_Button::OnButtonClicked()
{
	if(bSelected)
	{
		Image_Selection->SetRenderOpacity(0.f);
		bSelected = false;
		GetWidgetControllerSkillMenu(this)->DeselectSkillIcon();
		PlaySound(Sound_Cancel);
	}
	else
	{
		// TODO : 

		GetWidgetControllerSkillMenu(this)->SelectSkillIcon(TDAbilityTag);
	}

}
