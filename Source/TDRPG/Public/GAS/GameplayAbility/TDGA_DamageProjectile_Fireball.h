#pragma once
#include "CoreMinimal.h"
#include "GAS/GameplayAbility/TDGA_DamageProjectile.h"
#include "TDGA_DamageProjectile_Fireball.generated.h"

/**
 * 
 */
UCLASS()
class TDRPG_API UTDGA_DamageProjectile_Fireball : public UTDGA_DamageProjectile
{
	GENERATED_BODY()

public:
	virtual FString GetDescription(int32 AbilityLevel) override;
	virtual FString GetNextAbilityLevelDescription(int32 AbilityLevel) override;
};
