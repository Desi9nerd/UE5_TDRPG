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
			if (false == IsValid(PS))
			{
				UE_LOG(LogTemp, Warning, TEXT("PlayerState is NULL"));
				return nullptr;
			}

			UAbilitySystemComponent* ASC_Temp = PS->GetAbilitySystemComponent();
			if (false == IsValid(ASC_Temp))
			{
				// Ability System Component가 아직 초기화되지 않았을 경우 델리게이트 구독
				PS->OnAbilitySystemComponentInitialized.AddUniqueDynamic(this, &UTDUW::OnAbilitySystemComponentInitialized);
				return nullptr;
			}

			UAttributeSet* AS = PS->GetAttributeSet(); // Assuming GetAttributeSet() is defined and not NULL

			const FWidgetControllerParams WidgetControllerParams(PlayerController, PS, ASC_Temp, AS);

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
			if (!PS)
			{
				UE_LOG(LogTemp, Warning, TEXT("PlayerState is NULL"));
				return nullptr;
			}

			UAbilitySystemComponent* ASC_Temp = PS->GetAbilitySystemComponent();
			if (!ASC_Temp)
			{
				// Ability System Component가 아직 초기화되지 않았을 경우 델리게이트 구독
				PS->OnAbilitySystemComponentInitialized.AddUniqueDynamic(this, &UTDUW::OnAbilitySystemComponentInitialized);
				return nullptr;
			}

			UAttributeSet* AS = PS->GetAttributeSet(); // Assuming GetAttributeSet() is defined and not NULL
			const FWidgetControllerParams WidgetControllerParams(PlayerController, PS, ASC_Temp, AS);

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
			UAbilitySystemComponent* ASC_Temp = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet(); // Assuming GetAttributeSet() is defined and not NULL
			const FWidgetControllerParams WidgetControllerParams(PlayerController, PS, ASC_Temp, AS);

			return TDHUD->GetTDWidgetControllerSkillMenu(WidgetControllerParams);
		}
	}
	return nullptr;
}

void UTDUW::OnAbilitySystemComponentInitialized()
{
	// Ability System Component 초기화 완료 후 다시 호출
	GetWidgetControllerOverlay(GetWorld());
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