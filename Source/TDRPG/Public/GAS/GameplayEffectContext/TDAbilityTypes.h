#pragma once
#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "TDAbilityTypes.generated.h"

class UGameplayEffect;

/** FGameplayEffectContext�� Critical Hit, Block Hit, Debuff ��Ҹ� �߰��ϱ� ���� ����.
 *  GameplayEffectContext�� ��ӹ޾� GetScriptStruct, NetSerialize�� ������
 *	TDAbilitySystemGlobals���� AllocGameplayEffectContext �Լ� �������� ���� Ÿ���� FTDGameplayEffectContext���� ����� GAS���� FTDGameplayEffectContext�� ����ϵ��� ������.
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

	//** FGameplayEffectContext �Լ� ������
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
