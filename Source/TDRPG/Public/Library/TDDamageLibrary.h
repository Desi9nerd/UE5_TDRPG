#pragma once
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "TDDamageLibrary.generated.h"

class UAbilitySystemComponent;
class UGameplayEffect;

/** GAS에 활용할 데미지 관련정보를 묶은 구조체
 *  TDGA_Damage의 FDamageEffectParams SetDamageEffectParams()함수로 데미지 정보를 설정.
 */

USTRUCT(BlueprintType)
struct FDamageEffectParams
{
	GENERATED_BODY()

	FDamageEffectParams() {}

	UPROPERTY()
	TObjectPtr<UObject> WorldContextObject = nullptr;
	UPROPERTY()
	TSubclassOf<UGameplayEffect> GEDamageClass = nullptr;
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> SourceAbilitySystemComponent;
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> TargetAbilitySystemComponent;

	UPROPERTY()
	float BaseDamage = 0.f;
	UPROPERTY()
	float AbilityLevel = 1.f;
	UPROPERTY()
	FGameplayTag DamageType = FGameplayTag();
	UPROPERTY()
	float DebuffChance = 0.f;
	UPROPERTY()
	float DebuffDamage = 0.f;
	UPROPERTY()
	float DebuffDuration = 0.f;
	UPROPERTY()
	float DebuffFrequency = 0.f;
	UPROPERTY()
	float RagdollImpulseMagnitude = 0.f;
	UPROPERTY()
	FVector RagdollImpulse = FVector::ZeroVector;
	UPROPERTY(BlueprintReadWrite)
	float KnockbackImpulseMagnitude = 0.f;
	UPROPERTY(BlueprintReadWrite)
	float KnockbackChance = 0.f;
	UPROPERTY(BlueprintReadWrite)
	FVector KnockbackImpulse = FVector::ZeroVector;


	UPROPERTY(BlueprintReadWrite)
	bool bRadialDamage = false;
	UPROPERTY(BlueprintReadWrite)
	float RadialDamageInnerRadius = 0.f;
	UPROPERTY(BlueprintReadWrite)
	float RadialDamageOuterRadius = 0.f;
	UPROPERTY(BlueprintReadWrite)
	FVector RadialDamageOrigin = FVector::ZeroVector;
};
