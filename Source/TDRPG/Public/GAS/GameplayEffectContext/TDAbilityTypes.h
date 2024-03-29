#pragma once
#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "TDAbilityTypes.generated.h"

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