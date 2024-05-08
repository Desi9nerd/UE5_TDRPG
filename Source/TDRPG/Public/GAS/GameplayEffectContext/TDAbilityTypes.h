#pragma once
#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "TDAbilityTypes.generated.h"

class UGameplayEffect;

/** FGameplayEffectContext�� Critical Hit, Block Hit ��Ҹ� �߰��ϱ� ���� ����.
 *  GameplayEffectContext�� ��ӹ޾� GetScriptStruct, NetSerialize�� ������
 *	TDAbilitySystemGlobals���� AllocGameplayEffectContext �Լ� �������� ���� Ÿ���� FTDGameplayEffectContext���� ����� GAS���� FTDGameplayEffectContext�� ����ϵ��� ������.
 */

USTRUCT(BlueprintType)
struct FTDGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:
	bool IsCriticalHit() const { return bIsCriticalHit; }
	bool IsBlockedHit() const { return bIsBlockedHit; }

	void SetIsCriticalHit(bool bInIsCriticalHit) { bIsCriticalHit = bInIsCriticalHit; }
	void SetIsBlockedHit(bool bInIsBlockedHit) { bIsBlockedHit = bInIsBlockedHit; }

	//** FGameplayEffectContext �Լ� ������
	virtual UScriptStruct* GetScriptStruct() const;
	virtual FGameplayEffectContext* Duplicate() const;
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess); // Custom serialization

protected:
	UPROPERTY()
	bool bIsBlockedHit = false;

	UPROPERTY()
	bool bIsCriticalHit = false;

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
};
