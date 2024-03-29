#include "UI/Widget/TDUW_AttributeMenu.h"
#include "Kismet/GameplayStatics.h"
#include "Player/TDPlayerState.h"
#include "UI/HUD/TDHUD.h"
#include "UI/Widget/TDUW_TextValueRowButton.h"
#include "UI/WidgetController/TDWidgetControllerAttributeMenu.h"

void UTDUW_AttributeMenu::NativeConstruct()
{
	Super::NativeConstruct();


	UTDWidgetControllerAttributeMenu* WidgetControllerAttributeMenu = nullptr;
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0))
	{
		if (ATDHUD* tdHUD = Cast<ATDHUD>(PlayerController->GetHUD()))
		{
			ATDPlayerState* PS = PlayerController->GetPlayerState<ATDPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PlayerController, PS, ASC, AS);

			WidgetControllerAttributeMenu = tdHUD->GetWidgetControllerAttributeMenu(WidgetControllerParams);
		}
	}
	SetWidgetController(WidgetControllerAttributeMenu);
	WidgetControllerAttributeMenu->BroadcastInitialValues();
	
}

void UTDUW_AttributeMenu::SetAttributeTags()
{
}

void UTDUW_AttributeMenu::BroadcastInitialValues(UTDWidgetController& InTaget)
{
}
