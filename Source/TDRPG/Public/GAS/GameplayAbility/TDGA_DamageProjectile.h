#pragma once
#include "CoreMinimal.h"
#include "GAS/GameplayAbility/TDGA_Damage.h"
#include "TDGA_DamageProjectile.generated.h"

class ATDProjectile;
class UGameplayEffect;
struct FGameplayTag;

/** TDProjectile ���͸� ������ GameAbility
 *  Fireball ��ų
 */

UCLASS()
class TDRPG_API UTDGA_DamageProjectile : public UTDGA_Damage
{
	GENERATED_BODY()

public:
	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	//virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void SpawnProjectile(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, bool bOverridePitch = false, float PitchOverride = 0.f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ATDProjectile> ProjectileClass; // Projectile Actor

	UPROPERTY(EditDefaultsOnly)
	int32 NumProjectiles = 5;
};
