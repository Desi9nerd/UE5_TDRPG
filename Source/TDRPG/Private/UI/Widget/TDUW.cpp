#include "UI/Widget/TDUW.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Image.h"
#include "Components/OverlaySlot.h"
#include "Components/SizeBox.h"
#include "Kismet/GameplayStatics.h"
#include "UI/WidgetController/TDWidgetController.h"
#include "Player/TDPlayerState.h"
#include "UI/HUD/TDHUD.h"
#include "UI/WidgetController/TDWidgetControllerSkillMenu.h"

void UTDUW::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet(); // Blueprint에서 실행
}

UTDWidgetControllerOverlay* UTDUW::GetWidgetControllerOverlay(const UObject* WorldContextObject)
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (ATDHUD* tdHUD = Cast<ATDHUD>(PlayerController->GetHUD()))
		{
			ATDPlayerState* PS = PlayerController->GetPlayerState<ATDPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PlayerController, PS, ASC, AS);

			return tdHUD->GetTDWidgetControllerOverlay(WidgetControllerParams);
		}
	}
	return nullptr;
}

UTDWidgetControllerAttributeMenu* UTDUW::GetAttributeMenuWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (ATDHUD* tdHUD = Cast<ATDHUD>(PlayerController->GetHUD()))
		{
			ATDPlayerState* PS = PlayerController->GetPlayerState<ATDPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PlayerController, PS, ASC, AS);

			return tdHUD->GetTDWidgetControllerAttributeMenu(WidgetControllerParams);
		}
	}
	return nullptr;
}

UTDWidgetControllerSkillMenu* UTDUW::GetWidgetControllerSkillMenu(const UObject* WorldContextObject)
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (ATDHUD* TDHUD = Cast<ATDHUD>(PlayerController->GetHUD()))
		{
			ATDPlayerState* PS = PlayerController->GetPlayerState<ATDPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PlayerController, PS, ASC, AS);
			
			return TDHUD->GetTDWidgetControllerSkillMenu(WidgetControllerParams);
		}
	}
	return nullptr;
}

void UTDUW::UpdateBoxSize(USizeBox* OutSizeBox, int InWidth, int InHeight)
{
	OutSizeBox->SetWidthOverride(InWidth);
	OutSizeBox->SetHeightOverride(InHeight);
}

void UTDUW::UpdateBrush(UImage* OutImage, FSlateBrush InBrush)
{
	OutImage->SetBrush(InBrush);
}

void UTDUW::SetPadding(UImage* OutImage, float InLeft, float InRight, float InTop, float InBottom)
{
	FMargin Margin;
	Margin.Left = InLeft;
	Margin.Right = InRight;
	Margin.Top = InTop;
	Margin.Bottom = InBottom;

	UWidgetLayoutLibrary::SlotAsOverlaySlot(OutImage)->SetPadding(Margin);
}