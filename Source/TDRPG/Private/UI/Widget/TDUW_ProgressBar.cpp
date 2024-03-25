#include "UI/Widget/TDUW_ProgressBar.h"
#include "Components/SizeBox.h"
#include "Components/Button.h"
#include "Components/ProgressBar.h"
#include "Kismet/KismetMathLibrary.h"

void UTDUW_ProgressBar::NativePreConstruct()
{
	Super::NativePreConstruct();

	UpdateBoxSize();
	UpdateFrontFillBrush();
	UpdateGhostFillBrush();
}

void UTDUW_ProgressBar::NativeConstruct()
{
	Super::NativeConstruct();

	SetBarVisibility(false); // 생성 시 체력바 안 보이도록 설정
}

void UTDUW_ProgressBar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (bProgressBarVisibility)
	{
		InterpGhostBar(InDeltaTime);
	}
}

void UTDUW_ProgressBar::SetBarPercent(const float& InValue, const float& InMaxValue)
{
	SetBarVisibility(true);

	float InPercent = UKismetMathLibrary::SafeDivide(InValue, InMaxValue);
	ProgressBar_Front->SetPercent(InPercent);

	UpdateGhostInterpTarget(InPercent);
}

void UTDUW_ProgressBar::UpdateBoxSize()
{
	SizeBox_Root->SetWidthOverride(BoxWidth);
	SizeBox_Root->SetHeightOverride(BoxHeight);
}

void UTDUW_ProgressBar::UpdateFrontFillBrush()
{
	FSlateBrush SlateBrush;
	SlateBrush.TintColor = FSlateColor(FLinearColor(1.f, 1.f, 1.f, 0.f)); // Alpha값 0으로 설정

	FProgressBarStyle ProgressBarStyle;
	ProgressBarStyle.BackgroundImage = SlateBrush;
	ProgressBarStyle.FillImage = FrontBarFillBrush;

	ProgressBar_Front->SetWidgetStyle(ProgressBarStyle);
}

void UTDUW_ProgressBar::UpdateGhostFillBrush()
{
	FSlateBrush SlateBrush;
	SlateBrush.TintColor = FSlateColor(FLinearColor(1.f, 1.f, 1.f, 0.f)); 

	FProgressBarStyle ProgressBarStyle;
	ProgressBarStyle.BackgroundImage = SlateBrush;
	ProgressBarStyle.FillImage = GhostBarFillBrush;

	ProgressBar_Ghost->SetWidgetStyle(ProgressBarStyle);
}

void UTDUW_ProgressBar::SetBarVisibility(bool bVisible)
{
	bProgressBarVisibility = bVisible;
	if (bProgressBarVisibility)
	{
		SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		SetVisibility(ESlateVisibility::Hidden);
	}
}

void UTDUW_ProgressBar::InterpGhostBar(float InDeltaTime)
{
	float Percent = UKismetMathLibrary::FInterpTo(
		ProgressBar_Ghost->GetPercent(), GhostPercentTarget, InDeltaTime, 2.f);

	ProgressBar_Ghost->SetPercent(Percent);
}

void UTDUW_ProgressBar::UpdateGhostInterpTarget(const float& InTarget)
{
	//FTimerHandle TimerHandle;
	HideTimer.Invalidate();
	
	FTimerDelegate TimerDelegate = FTimerDelegate::CreateLambda([this, InTarget]() {
		HideProgressBar(InTarget);
		});
	
	GetWorld()->GetTimerManager().SetTimer(HideTimer, TimerDelegate, 5.f, false);
}

void UTDUW_ProgressBar::HideProgressBar(const float& InTarget)
{
	SetBarVisibility(false);

	FTimerHandle GhostDelayTimer;

	FTimerDelegate TimerDelegate = FTimerDelegate::CreateLambda([this, InTarget]() {
		DelayGhostPercentTarget(InTarget);
		});

	GetWorld()->GetTimerManager().SetTimer(GhostDelayTimer, TimerDelegate, 1.f, false);
}

void UTDUW_ProgressBar::DelayGhostPercentTarget(const float& InTarget)
{
	GhostPercentTarget = InTarget;
}
