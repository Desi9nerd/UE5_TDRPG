#pragma once
#include "CoreMinimal.h"
#include "GAS/GameplayAbility/TDGA_Damage.h"
#include "TDGA_DamagePillarOfFlame.generated.h"

class ATDPillarOfFlamesActor;
/**
 * 
 */
UCLASS()
class TDRPG_API UTDGA_DamagePillarOfFlame : public UTDGA_Damage
{
	GENERATED_BODY()
	
public:

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TSubclassOf<ATDPillarOfFlamesActor> SpawnActorClass;
	
};
