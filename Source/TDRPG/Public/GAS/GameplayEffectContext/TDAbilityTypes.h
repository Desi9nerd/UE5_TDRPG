#pragma once
#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "TDAbilityTypes.generated.h"

/** Critical Hit, Block Hit
 *  GameplayEffectContext�� ��ӹ޾� GetScriptStruct, NetSerialize�� ������
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
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess); // Custom serialization

protected:
	UPROPERTY()
	bool bIsBlockedHit = false;

	UPROPERTY()
	bool bIsCriticalHit = false;

};