#pragma once
#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "TDWidgetComponent.generated.h"

/** �� ĳ���͸� ���������� ȭ�鿡 ������ ������ �ؽ�Ʈ ������ ���
 * 
 */
UCLASS()
class TDRPG_API UTDWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetDamageText(float Damage);
};
