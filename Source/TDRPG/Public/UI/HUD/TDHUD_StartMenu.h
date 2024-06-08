#pragma once
#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TDHUD_StartMenu.generated.h"

class UTDMVVM_StartMenu;
class UTDUW_LoadScreen;

/** 게임 처음 시작 시 진입하는 레벨에서 사용하는 HUD
 * 
 */

UCLASS()
class TDRPG_API ATDHUD_StartMenu : public AHUD
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> LoadScreenWidgetClass;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UTDUW_LoadScreen> LoadScreenWidget;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UTDMVVM_StartMenu> LoadScreenViewModelClass;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UTDMVVM_StartMenu> LoadScreenViewModel;

protected:
	virtual void BeginPlay() override;

};
