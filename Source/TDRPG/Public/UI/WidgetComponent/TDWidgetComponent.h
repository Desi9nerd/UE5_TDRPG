#pragma once
#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "TDWidgetComponent.generated.h"

/** 적 캐릭터를 공격했을때 화면에 나오는 데미지 텍스트 구현에 사용
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
