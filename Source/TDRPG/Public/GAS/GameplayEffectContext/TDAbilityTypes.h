#pragma once
#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "TDAbilityTypes.generated.h"

class UGameplayEffect;

/** FGameplayEffectContext에 Critical Hit, Block Hit, Debuff 요소를 추가하기 위해 만듬.
 *  GameplayEffectContext를 상속받아 GetScriptStruct, NetSerialize를 재정의
 *	TDAbilitySystemGlobals에서 AllocGameplayEffectContext 함수 재정의의 리턴 타입을 FTDGameplayEffectContext으로 만들어 GAS에서 FTDGameplayEffectContext를 사용하도록 수정함.
 */

USTRUCT(BlueprintType)
struct FTDGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:
	FORCEINLINE float GetDebuffDamage() const { return DebuffDamage; }
	FORCEINLINE float GetDebuffDuration() const { return DebuffDuration; }
	FORCEINLINE float GetDebuffFrequency() const { return DebuffFrequency; }

	FORCEINLINE void SetCriticalHit(bool bInCriticalHit) { bCriticalHit = bInCriticalHit; }
	FORCEINLINE void SetBlockedHit(bool bInBlockedHit) { bBlockedHit = bInBlockedHit; }
	FORCEINLINE void SetDebuf(bool bInDebuff) { bDebuff = bInDebuff; }

	FORCEINLINE bool IsCriticalHit() const { return bCriticalHit; }
	FORCEINLINE bool IsBlockedHit() const { return bBlockedHit; }
	FORCEINLINE bool IsDebuff() const { return bDebuff; }

	//** FGameplayEffectContext 함수 재정의
	virtual UScriptStruct* GetScriptStruct() const;
	virtual FGameplayEffectContext* Duplicate() const;
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess); // Custom serialization

protected:
	UPROPERTY()
	bool bBlockedHit = false;
	UPROPERTY()
	bool bCriticalHit = false;

	UPROPERTY()
	bool bDebuff = false;
	UPROPERTY()
	float DebuffDamage = 0.f;
	UPROPERTY()
	float DebuffDuration = 0.f;
	UPROPERTY()
	float DebuffFrequency = 0.f;

	TSharedPtr<FGameplayTag> DamageType;
};

template<>
struct TStructOpsTypeTraits<FTDGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FTDGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true
	};
};
