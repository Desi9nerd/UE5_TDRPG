#include "UI/Widget/TDUW_SkillIcon.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "GameplayTags/TDGameplayTags.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetTextLibrary.h"
#include "UI/WidgetController/TDWidgetControllerOverlay.h"

void UTDUW_SkillIcon::NativePreConstruct()
{
	UpdateBoxSize(SizeBox_Root, BoxWidth, BoxHeight);
	//UpdateBrush(Image_Background, BackgroundBrush);
	//UpdateBrush(Image_Ring, RingBrush);
	//UpdateBrush(Image_SkillIcon, SkillIconBrush);
	Image_Background->SetBrush(TransparentBrush);
	Image_SkillIcon->SetBrush(TransparentBrush);
	Image_Ring->SetBrush(TransparentBrush);
	SetPadding(Image_Background, Padding_Background, Padding_Background, Padding_Background, Padding_Background);
	SetPadding(Image_SkillIcon, Padding_SkillIcon, Padding_SkillIcon, Padding_SkillIcon, Padding_SkillIcon);

	Text_Cooldown->SetRenderOpacity(0.f); // Cooldown �ؽ�Ʈ �����
}

void UTDUW_SkillIcon::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(GetWidgetControllerOverlay(this)))
	{
		GetWidgetControllerOverlay(this)->DA_AbilityInfoDelegate.AddDynamic(this, &ThisClass::OnAbilityInfoDelegate_Event);
	}
}

void UTDUW_SkillIcon::NativeDestruct()
{
	Super::NativeDestruct();

	if (IsValid(TDWidgetControllerOverlay))
	{
		// TODO : Unbind all Event from DA_AbilityInfo Delegate

		//TDWidgetControllerOverlay->OnHealthChanged.Clear();
	}
}

void UTDUW_SkillIcon::OnAbilityInfoDelegate_Event(const FDA_Ability& DA_Ability)
{
	ReceiveDA_Ability(DA_Ability);

	if (IsValid(AsyncTask_WaitCooldown))
	{
		AsyncTask_WaitCooldown->EndTask();
	}

	AsyncTask_WaitCooldown = AsyncTask_WaitCooldown->WaitForCooldownChange(TDWidgetControllerOverlay->GetASC(), CooldownTag);

	
}

void UTDUW_SkillIcon::OnUpdateTimer()
{
	TimeRemaining -= TimerFrequency; // �����ð� -= 0.1�Ͽ� �ð��� �پ��°��� �����ش�.
	TimeRemaining = UKismetMathLibrary::Clamp(TimeRemaining, 0, TimeRemaining);
	
	FText TimeRemainingText = FText::FromString(FString::SanitizeFloat(TimeRemaining));
	Text_Cooldown->SetText(TimeRemainingText);

	if (TimeRemaining <= 0.f) 
	{
		GetWorld()->GetTimerManager().ClearTimer(CooldownTimerHandle); // ��Ÿ�� 0 ���ϸ� Handle ���ֱ�.

		Image_Background->SetBrushTintColor(FSlateColor(FLinearColor(1.f, 1.f, 1.f, 1.f))); // Background�̹��� ���̰� �ϱ�.
		Text_Cooldown->SetRenderOpacity(0.f); // ���� �� ���̰� �ϱ�.
	}
}

void UTDUW_SkillIcon::ReceiveDA_Ability(FDA_Ability InDA_Ability)
{
	if (InDA_Ability.InputTag.MatchesTagExact(InputTag))
	{
		if (InDA_Ability.AbilityTag.MatchesTagExact(FTDGameplayTags::GetTDGameplayTags().Abilities_None))
		{
			// TObjectPtr���� ���������� ���·� ��ȯ. FDA_Ability���� const�� �����ؼ� const�� ���ִ� ��� const_cast ���.
			UMaterialInterface* Material_Background = const_cast<UMaterialInterface*>(InDA_Ability.BackgroundMaterial.Get());
			UTexture2D* Texture_Icon = const_cast<UTexture2D*>(InDA_Ability.Icon.Get());
			Image_Background->SetBrush(UWidgetBlueprintLibrary::MakeBrushFromMaterial(Material_Background));
			Image_SkillIcon->SetBrush(UWidgetBlueprintLibrary::MakeBrushFromTexture(Texture_Icon));

			CooldownTag = InDA_Ability.CooldownTag;
		}
		else
		{
			Image_Background->SetBrush(TransparentBrush);
			Image_SkillIcon->SetBrush(TransparentBrush);
			CooldownTag = FGameplayTag::EmptyTag;
		}
	}
}

void UTDUW_SkillIcon::SetCooldownState()
{
	FLinearColor SpecifiedColor = FLinearColor(CooldownTint, CooldownTint, CooldownTint, 1.f);
	Image_Background->SetBrushTintColor(FSlateColor(SpecifiedColor));

	Text_Cooldown->SetRenderOpacity(1.f);

	FTimerDelegate CooldownTimerDelegate = FTimerDelegate::CreateLambda([this]() {
		OnUpdateTimer();
		});
	GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle, CooldownTimerDelegate, TimerFrequency, true);
}
