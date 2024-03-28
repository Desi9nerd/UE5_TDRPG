#pragma once
#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "TDGEEC_Damage.generated.h"

/** GameplayEffect ������ ������ ��꿡 ���
 *  GameplayEffect�� ���ϰ� �Ǵ� Ŭ����. GE_Damage�� ����Ͽ� ���
 *	MMC�� �޸� �������� Attributes�� ��� �� ������ �� �ִ�
 *	����� ���ÿ��� �̷����� ���� ���� �������� ���� ü���� �����Ǹ� ü�� ������ ������ �����ָ� �ȴ�.
 */

UCLASS()
class TDRPG_API UTDGEEC_Damage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UTDGEEC_Damage();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
