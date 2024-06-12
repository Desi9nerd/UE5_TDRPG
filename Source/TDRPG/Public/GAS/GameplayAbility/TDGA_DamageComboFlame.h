#pragma once
#include "CoreMinimal.h"
#include "GAS/GameplayAbility/TDGA_Damage.h"
#include "TDGA_DamageComboFlame.generated.h"

class ATDProjectileFlame;
/**
 * 
 */
UCLASS()
class TDRPG_API UTDGA_DamageComboFlame : public UTDGA_Damage
{
	GENERATED_BODY()

public:
	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

	UFUNCTION(BlueprintCallable)
	TArray<ATDProjectileFlame*> SpawnFlames();

protected:
	UFUNCTION(BlueprintCallable)
	void SpawnProjectile(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, bool bOverridePitch = false, float PitchOverride = 0.f);

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag SocketTag1;
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag SocketTag2;
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag SocketTag3;
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag SocketTag4;

	UPROPERTY(EditDefaultsOnly)
	int32 NumOfProjectiles = 6;

private:
	TArray<FRotator> EvenlySpacedRotators(const FVector& ForwardVector, const FVector& Axis, float Spread, int32 NumOfRotators);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ATDProjectileFlame> ProjectileClass; // Projectile Actor

};
