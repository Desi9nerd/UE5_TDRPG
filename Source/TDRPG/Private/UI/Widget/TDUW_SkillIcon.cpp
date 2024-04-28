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

	Text_Cooldown->SetRenderOpacity(0.f); // Cooldown 텍스트 숨기기
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
	TimeRemaining -= TimerFrequency; // 남은시간 -= 0.1하여 시간이 줄어드는것을 보여준다.
	TimeRemaining = UKismetMathLibrary::Clamp(TimeRemaining, 0, TimeRemaining);
	
	FText TimeRemainingText = FText::FromString(FString::SanitizeFloat(TimeRemaining));
	Text_Cooldown->SetText(TimeRemainingText);

	if (TimeRemaining <= 0.f) 
	{
		GetWorld()->GetTimerManager().ClearTimer(CooldownTimerHandle); // 쿨타임 0 이하면 Handle 없애기.

		Image_Background->SetBrushTintColor(FSlateColor(FLinearColor(1.f, 1.f, 1.f, 1.f))); // Background이미지 보이게 하기.
		Text_Cooldown->SetRenderOpacity(0.f); // 글자 안 보이게 하기.
	}
}

void UTDUW_SkillIcon::ReceiveDA_Ability(FDA_Ability InDA_Ability)
{
	if (InDA_Ability.InputTag.MatchesTagExact(InputTag))
	{
		if (InDA_Ability.AbilityTag.MatchesTagExact(FTDGameplayTags::GetTDGameplayTags().Abilities_None))
		{
			// TObjectPtr에서 원시포인터 형태로 변환. FDA_Ability에서 const로 선언해서 const를 없애는 대신 const_cast 사용.
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
