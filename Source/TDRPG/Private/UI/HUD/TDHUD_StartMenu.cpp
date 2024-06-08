#include "UI/HUD/TDHUD_StartMenu.h"
#include "Blueprint/UserWidget.h"
#include "UI/MVVM/TDMVVM_StartMenu.h"
#include "UI/Widget/StartScreen/TDUW_LoadScreen.h"

void ATDHUD_StartMenu::BeginPlay()
{
	Super::BeginPlay();

	//* UTDMVVM_StartMenu 생성. 게임정보가 저장될 슬롯 3개를 초기화.
	LoadScreenViewModel = NewObject<UTDMVVM_StartMenu>(this, LoadScreenViewModelClass);
	LoadScreenViewModel->InitializeLoadSlots();

	//* 위젯 생성 후 화면에 띄우기. InitializeWidgetBP로 초기화.
	LoadScreenWidget = CreateWidget<UTDUW_LoadScreen>(GetWorld(), LoadScreenWidgetClass);
	LoadScreenWidget->AddToViewport();
	LoadScreenWidget->InitializeWidgetBP();

	LoadScreenViewModel->LoadData(); // 데이터 가져오기.
}
