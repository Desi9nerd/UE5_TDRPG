﻿#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "Library/TDDamageLibrary.h"
#include "TDProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UNiagaraSystem;

/** 스킬로 날아가는 액터
 *  CollisionObjectType을 ECC_Projectile 설정하였고 상대방은 해당 채널의 충돌설정을 Overlap 시에만 피격된다.
 *	TDGA_DamageProjectile에서 이 클래스 내의 FDamageEffectParams DamageEffectParams를 가져다가 사용함.
 */

UCLASS()
class TDRPG_API ATDProjectile : public AActor
{
	GENERATED_BODY()
	
public:
	ATDProjectile();

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;
	UPROPERTY()
	TObjectPtr<USceneComponent> TargetSceneComponent;

	// 방법1: GameplayEffectSpecHandle
	//UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	//FGameplayEffectSpecHandle DamageEffectSpecHandle;

	// 방법2: DamageEffectParams
	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	FDamageEffectParams DamageEffectParams;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable)
	void OnHit();
	virtual void Destroyed() override;

	bool IsValidOverlap(AActor* OtherActor);

	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USphereComponent> Sphere;

	UPROPERTY(EditDefaultsOnly)
	float LifeSpan = 8.f;

	bool bHit = false; // 피격 여부 true/false
private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> ImpactEffect;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UParticleSystem> ImpactEffect_ParticleSystem;
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> ImpactSound;
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> LoopingSound;
	UPROPERTY()
	TObjectPtr<UAudioComponent> LoopingSoundComponent;

	FVector LocationLastFrame;
	FVector LocationThisFrame;
};
