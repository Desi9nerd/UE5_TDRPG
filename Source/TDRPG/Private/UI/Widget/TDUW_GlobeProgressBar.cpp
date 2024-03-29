#include "UI/Widget/TDUW_GlobeProgressBar.h"
#include "Components/ProgressBar.h"
#include "Components/SizeBox.h"
#include "Components/Image.h"
#include "Kismet/KismetMathLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/OverlaySlot.h"

void UTDUW_GlobeProgressBar::NativePreConstruct()
{
	Super::NativePreConstruct();

	UpdateBoxSize();
	UpdateBackgrondBrush();
	UpdateGlobeBrush();
	UpdateGlobePadding();
	UpdateGlassBrush();
	UpdateGlassPadding();
	UpdateGhostGlobeBrush();
}

void UTDUW_GlobeProgressBar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	float Percent = UKismetMathLibrary::FInterpTo(ProgressBar_Ghost->GetPercent(), GhostPercentTarget, InDeltaTime, GhostInterpSpeed);

	SetGhostProgressBarPercent(Percent);
}

void UTDUW_GlobeProgressBar::SetProgressBarPercent(float InPercent)
{
	float ProgressPercent = InPercent;

	if (bGlobeInitialized)
	{
		ProgressBar_Globe->SetPercent(ProgressPercent);
		GlobePercentSet(ProgressPercent);
	}
	else
	{
		if (ProgressPercent > 0.f)
		{
			bGlobeInitialized = true;
			ProgressBar_Globe->SetPercent(ProgressPercent);
			ProgressBar_Ghost->SetPercent(ProgressPercent);
			GhostPercentTarget = ProgressPercent;
		}
	}
}

void UTDUW_GlobeProgressBar::GlobePercentSet(float InPercent)
{
	FTimerHandle GhostDelayTimer;

	FTimerDelegate TimerDelegate = FTimerDelegate::CreateLambda([this, InPercent]() {
		SetNewGhostPercentTarget(InPercent);
		});

	GetWorld()->GetTimerManager().SetTimer(GhostDelayTimer, TimerDelegate, GhostDelay, false); // GhostDelay초 지난 후 GhostPercentTarget 값 업데이트
}

void UTDUW_GlobeProgressBar::SetNewGhostPercentTarget(float InPercent)
{
	GhostPercentTarget = InPercent;
}

void UTDUW_GlobeProgressBar::UpdateBoxSize()
{
	SizeBox_Root->SetWidthOverride(BoxWidth);
	SizeBox_Root->SetHeightOverride(BoxHeight);
}

void UTDUW_GlobeProgressBar::UpdateBackgrondBrush()
{
	Image_Background->SetBrush(BackgroundBrush);
}

void UTDUW_GlobeProgressBar::UpdateGlobeBrush()
{
	FSlateBrush SlateBrush;
	SlateBrush.TintColor = FSlateColor(FLinearColor(1.f, 1.f, 1.f, 0.f));

	FProgressBarStyle ProgressBarStyle;
	ProgressBarStyle.BackgroundImage = SlateBrush;
	ProgressBarStyle.FillImage = ProgressBarBrush; // MI_SoulGlobe

	ProgressBar_Globe->SetWidgetStyle(ProgressBarStyle);
}

void UTDUW_GlobeProgressBar::UpdateGlobePadding()
{
	FMargin Margin;
	Margin.Top = GlobePadding;
	Margin.Left = GlobePadding;
	Margin.Right = GlobePadding;
	Margin.Bottom = GlobePadding;

	UWidgetLayoutLibrary::SlotAsOverlaySlot(ProgressBar_Globe)->SetPadding(Margin);
}

void UTDUW_GlobeProgressBar::UpdateGlassBrush()
{
	Image_Glass->SetBrush(GlassBrush);
}

void UTDUW_GlobeProgressBar::UpdateGlassPadding()
{
	FMargin Margin;
	Margin.Top = GlobePadding;
	Margin.Left = GlobePadding;
	Margin.Right = GlobePadding;
	Margin.Bottom = GlobePadding;

	UWidgetLayoutLibrary::SlotAsOverlaySlot(Image_Glass)->SetPadding(Margin);
}

void UTDUW_GlobeProgressBar::UpdateGhostGlobeBrush()
{
	FSlateBrush SlateBrush;
	SlateBrush.TintColor = FSlateColor(FLinearColor(1.f, 1.f, 1.f, 0.f));

	FProgressBarStyle ProgressBarStyle;
	ProgressBarStyle.BackgroundImage = SlateBrush;
	ProgressBarStyle.FillImage = GhostProgressBarBrush;  // MI_GhostSoulGlobe

	ProgressBar_Ghost->SetWidgetStyle(ProgressBarStyle);
}

void UTDUW_GlobeProgressBar::SetGhostProgressBarPercent(const float& InPercent)
{
	ProgressBar_Ghost->SetPercent(InPercent);
}
