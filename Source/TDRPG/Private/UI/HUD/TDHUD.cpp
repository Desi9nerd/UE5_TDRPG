#include "UI/HUD/TDHUD.h"
#include "UI/Widget/TDUW.h"
#include "UI/Widget/Inventory/TDUW_Inventory.h"
#include "UI/WidgetController/TDWidgetControllerAttributeMenu.h"
#include "UI/WidgetController/TDWidgetControllerOverlay.h"
#include "UI/WidgetController/TDWidgetControllerSkillMenu.h"

TObjectPtr<UTDWidgetControllerOverlay> ATDHUD::GetTDWidgetControllerOverlay(const FWidgetControllerParams& WCParams)
{
	if (false == IsValid(TDWidgetController_Overlay)) // OverlayWidgetController이 없다면 설정
	{
		TDWidgetController_Overlay = NewObject<UTDWidgetControllerOverlay>(this, TDWidgetController_OverlayClass);
		TDWidgetController_Overlay->SetWidgetControllerParams(WCParams);
		TDWidgetController_Overlay->BindCallbacksToDependencies(); // 콜백함수 바인딩
	}

	return TDWidgetController_Overlay;
}

TObjectPtr<UTDWidgetControllerAttributeMenu> ATDHUD::GetTDWidgetControllerAttributeMenu(const FWidgetControllerParams& WCParams)
{
	if (false == IsValid(TDWidgetController_AttributeMenu))
	{
		TDWidgetController_AttributeMenu = NewObject<UTDWidgetControllerAttributeMenu>(this, TDWidgetController_AttributeMenuClass);
		TDWidgetController_AttributeMenu->SetWidgetControllerParams(WCParams);
		TDWidgetController_AttributeMenu->BindCallbacksToDependencies();
	}

	return TDWidgetController_AttributeMenu;
}

TObjectPtr<UTDWidgetControllerSkillMenu> ATDHUD::GetTDWidgetControllerSkillMenu(const FWidgetControllerParams& WCParams)
{
	if (false == IsValid(TDWidgetController_SkillMenu))
	{
		TDWidgetController_SkillMenu = NewObject<UTDWidgetControllerSkillMenu>(this, TDWidgetController_SkillMenuClass);
		TDWidgetController_SkillMenu->SetWidgetControllerParams(WCParams);
		TDWidgetController_SkillMenu->BindCallbacksToDependencies();
	}

	return TDWidgetController_SkillMenu;
}

void ATDHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	//** checkf는 데이터 null체크 후 crash가 나면 작성한 TEXT를 출력한다.
	checkf(OverlayWidgetClass, TEXT("Overlay Widget Class가 초기화되지 않았습니다.BP_TDHUD에 데이터를 넣어주세요."));
	checkf(TDWidgetController_OverlayClass, TEXT("Overlay Widget Controller Class가 초기화되지 않았습니다. BP_TDHUD에 데이터를 넣어주세요."));

	//** TDUserWidget
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<UTDUW>(Widget);

	//** TDOverlayWidgetController
	const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
	UTDWidgetControllerOverlay* WidgetController = GetTDWidgetControllerOverlay(WidgetControllerParams);

	OverlayWidget->SetWidgetController(WidgetController); // TDUserWidget에 TDOverlayWidgetController를 묶는다

	WidgetController->BroadcastInitialValues(); // TDOverlayWidgetController에 Dynamic Broadcast된 데이터를 불러옴
	Widget->AddToViewport(); // 위젯을 Viewport에 추가
}

TObjectPtr<UTDUW_Inventory> ATDHUD::GetInventoryWidget()
{
	if (IsValid(InventoryWidget)) return InventoryWidget;

	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), InventoryWidgetClass);
	InventoryWidget = Cast<UTDUW_Inventory>(Widget);
	InventoryWidget->AddToViewport();

	return InventoryWidget;
}
