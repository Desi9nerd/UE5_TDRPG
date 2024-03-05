#include "UI/HUD/TDHUD.h"
#include "UI/Widget/TDUserWidget.h"
#include "UI/WidgetController/TDOverlayWidgetController.h"

UTDOverlayWidgetController* ATDHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if (false == IsValid(OverlayWidgetController)) // OverlayWidgetController이 없다면 설정
	{
		OverlayWidgetController = NewObject<UTDOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams);

		return OverlayWidgetController;
	}

	return OverlayWidgetController;
}

void ATDHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	//** checkf는 데이터 null체크 후 crash가 나면 작성한 TEXT를 출력한다.
	checkf(OverlayWidgetClass, TEXT("Overlay Widget Class가 초기화되지 않았습니다.BP_TDHUD에 데이터를 넣어주세요."));
	checkf(OverlayWidgetControllerClass, TEXT("Overlay Widget Controller Class가 초기화되지 않았습니다. BP_TDHUD에 데이터를 넣어주세요."));

	//** TDUserWidget
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<UTDUserWidget>(Widget);

	//** TDOverlayWidgetController
	const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
	UTDOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);

	OverlayWidget->SetWidgetController(WidgetController); // TDUserWidget에 TDOverlayWidgetController를 묶는다

	WidgetController->BroadcastInitialValues(); // TDOverlayWidgetController에 Dynamic Broadcast된 데이터를 불러옴
	Widget->AddToViewport(); // 위젯을 Viewport에 추가
}