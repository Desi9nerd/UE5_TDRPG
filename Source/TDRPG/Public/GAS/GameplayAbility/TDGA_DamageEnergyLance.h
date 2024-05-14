#pragma once
#include "CoreMinimal.h"
#include "TDGA_Damage.h"
#include "TDGA_DamageEnergyLance.generated.h"

/** ���콺�� ����Ű�� �������� ���ʸ����ĸ� ����. Ű�� ������ ���� ����ؼ� ���ʸ����İ� ����.
 *  ���콺�� ����Ų ������ ������ ����, ���� �ݰ濡 ���� �� ������ �ش� ���鿡�Ե� ������ ��.
 *  TDGC_Notify_Loop�� GameplayCue_Loop �±׷� ���� �� �Ҹ� ��Ŵ
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
	//** Ÿ��(=��) �����ϴ� �Լ���.
	UFUNCTION(BlueprintCallable)
	void TraceFirstTarget(const FVector& BeamTargetLocation);
	UFUNCTION(BlueprintCallable)
	void StoreAdditionalTargets(TArray<AActor*>& OutAdditionalTargets);
	//*****************************************************

	//*****************************************************
	//** Ÿ��(=��) ���� ó�� �Լ���.
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
