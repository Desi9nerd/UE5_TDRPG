#pragma once
#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "TDAbilitySystemGlobals.generated.h"

/** AllocGameplayEffectContext 재정의 함수의 리턴 타입을 FTDGameplayEffectContext으로 만듬. FGameplayEffectContext가 아닌 Critical Hit, Block Hit 요소가 추가된 FTDGameplayEffectContext()를 사용하게 함.
 * 
 */
UCLASS()
class TDRPG_API UTDAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()

public:
	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
};
