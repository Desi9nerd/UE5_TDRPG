#pragma once
#include "CoreMinimal.h"
#include "TDGA_Damage.h"
#include "TDGA_DamageEnergyLance.generated.h"

/**
 * 
 */
UCLASS()
class TDRPG_API UTDGA_DamageEnergyLance : public UTDGA_Damage
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void StoreMouseDataInfo(const FHitResult& HitResult);

	UFUNCTION(BlueprintCallable)
	void StoreOwnerVariables();

protected:
	UPROPERTY(BlueprintReadWrite)
	FVector MouseHitLocation;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<AActor> MouseHitActor;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<APlayerController> OwnerPlayerController;
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<ACharacter> OwnerCharacter;

private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess))
	TObjectPtr<UAnimMontage> EnergyLanceMontage;
};
