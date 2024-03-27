#pragma once
#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "TDGEEC_Damage.generated.h"

/** GameplayEffect 내에서 데미지 계산에 사용
 *  GameplayEffect에 속하게 되는 클래스
 */

UCLASS()
class TDRPG_API UTDGEEC_Damage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UTDGEEC_Damage();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
