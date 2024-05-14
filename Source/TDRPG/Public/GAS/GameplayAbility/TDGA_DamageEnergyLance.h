#pragma once
#include "CoreMinimal.h"
#include "TDGA_Damage.h"
#include "TDGA_DamageEnergyLance.generated.h"

/** 마우스가 가리키는 방향으로 에너르기파를 날림. 키를 누르는 동안 계속해서 에너르기파가 나감.
 *  마우스가 가리킨 적에게 공격이 들어가고, 일정 반경에 적이 더 있으면 해당 적들에게도 공격이 들어감.
 *  TDGC_Notify_Loop를 GameplayCue_Loop 태그로 생성 및 소멸 시킴
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

	//*****************************************************
	//** 타겟(=적) 설정하는 함수들.
	UFUNCTION(BlueprintCallable)
	void TraceFirstTarget(const FVector& BeamTargetLocation);
	UFUNCTION(BlueprintCallable)
	void StoreAdditionalTargets(TArray<AActor*>& OutAdditionalTargets);
	//*****************************************************

	//*****************************************************
	//** 타겟(=적) 죽음 처리 함수들.
	UFUNCTION(BlueprintImplementableEvent)
	void PrimaryTargetDiedBP(AActor* DeadActor);
	void PrimaryTargetDied(AActor* DeadActor);
	UFUNCTION(BlueprintImplementableEvent)
	void AdditionalTargetDiedBP(AActor* DeadActor);
	void AdditionalTargetDied(AActor* DeadActor);
	//*****************************************************

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
	void RegisterOnDeathDelegate(AActor* InActor);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TObjectPtr<UAnimMontage> EnergyLanceMontage;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess))
	int32 MaxNumShockTargets = 5;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess))
	float AdditionalTargetAcceptRadius = 800.f;
};
