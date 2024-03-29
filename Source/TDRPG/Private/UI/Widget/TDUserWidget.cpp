#include "UI/Widget/TDUserWidget.h"

#include "Kismet/GameplayStatics.h"
#include "UI/WidgetController/TDWidgetController.h"
#include "Player/TDPlayerState.h"
#include "UI/HUD/TDHUD.h"

void UTDUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet(); // Blueprint에서 실행
}

UTDWidgetControllerOverlay* UTDUserWidget::GetWidgetControllerOverlay(const UObject* WorldContextObject)
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (ATDHUD* tdHUD = Cast<ATDHUD>(PlayerController->GetHUD()))
		{
			ATDPlayerState* PS = PlayerController->GetPlayerState<ATDPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PlayerController, PS, ASC, AS);

			return tdHUD->GetWidgetControllerOverlay(WidgetControllerParams);
		}
	}
	return nullptr;
}

UTDWidgetControllerAttributeMenu* UTDUserWidget::GetAttributeMenuWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (ATDHUD* tdHUD = Cast<ATDHUD>(PlayerController->GetHUD()))
		{
			ATDPlayerState* PS = PlayerController->GetPlayerState<ATDPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PlayerController, PS, ASC, AS);

			return tdHUD->GetWidgetControllerAttributeMenu(WidgetControllerParams);
		}
	}
	return nullptr;
}
