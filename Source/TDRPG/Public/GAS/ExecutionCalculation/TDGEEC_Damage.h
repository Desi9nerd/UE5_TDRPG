#pragma once
#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "TDGEEC_Damage.generated.h"

/** GameplayEffect 내에서 데미지 계산에 사용
 *  GameplayEffect에 속하게 되는 클래스. GE_Damage에 등록하여 사용
 *	MMC와 달리 여러개의 Attributes를 계산 및 수정할 수 있다
 *	계산은 로컬에서 이뤄지며 최종 계산된 데미지만 보낸다. 데미지를 건네받은 TDAttributeset은 데미지만큼 체력을 차감하면 체력 정보만 서버에 보내주면 된다.
 */

UCLASS()
class TDRPG_API UTDGEEC_Damage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UTDGEEC_Damage();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
