#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TDWidgetController.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;

/** 위젯과 데이터를 관리하는 클래스 사이의 교량역할
 *  위젯의 변경사항(ex.버튼클릭)을 클래스에 알려주는 역할
 */
UCLASS()
class TDRPG_API UTDWidgetController : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<APlayerController> PlayerController;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<APlayerState> PlayerState;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UAttributeSet> AttributeSet;
};
