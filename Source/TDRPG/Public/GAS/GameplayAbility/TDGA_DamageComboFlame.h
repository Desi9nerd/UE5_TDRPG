#pragma once
#include "CoreMinimal.h"
#include "GAS/GameplayAbility/TDGA_DamageCombo.h"
#include "TDGA_DamageComboFlame.generated.h"

class ATDProjectile;
/**
 * 
 */
UCLASS()
class TDRPG_API UTDGA_DamageComboFlame : public UTDGA_DamageCombo
{
	GENERATED_BODY()

public:
	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

protected:
	UFUNCTION(BlueprintCallable)
	void SpawnProjectile(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, bool bOverridePitch = false, float PitchOverride = 0.f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ATDProjectile> ProjectileClass; // Projectile Actor

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag SocketTag1;
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag SocketTag2;
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag SocketTag3;
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag SocketTag4;

};
