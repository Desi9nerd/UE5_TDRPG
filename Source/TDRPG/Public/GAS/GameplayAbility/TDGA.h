#pragma once
#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "TDGA.generated.h"

/**
 * 
 */
UCLASS()
class TDRPG_API UTDGA : public UGameplayAbility
{
	GENERATED_BODY()

public:
	// InputTag�� ���� �� ��Ÿ�ӿ� ����� ���� �ִ�
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	FGameplayTag StartupInputTag; // ���ӽ��� �� InputTag

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	FScalableFloat Damage;
};
