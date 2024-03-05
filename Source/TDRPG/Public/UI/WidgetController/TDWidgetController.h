#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TDWidgetController.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;

/** ������ �����͸� �����ϴ� Ŭ���� ������ ��������
 *  ������ �������(ex.��ưŬ��)�� Ŭ������ �˷��ִ� ����
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
