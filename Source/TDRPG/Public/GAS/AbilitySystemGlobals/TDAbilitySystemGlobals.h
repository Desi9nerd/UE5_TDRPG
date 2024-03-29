#pragma once
#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "TDAbilitySystemGlobals.generated.h"

/** AllocGameplayEffectContext ������ �Լ��� ���� Ÿ���� FTDGameplayEffectContext���� ����. FGameplayEffectContext�� �ƴ� Critical Hit, Block Hit ��Ұ� �߰��� FTDGameplayEffectContext()�� ����ϰ� ��.
 * 
 */
UCLASS()
class TDRPG_API UTDAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()

public:
	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
};
