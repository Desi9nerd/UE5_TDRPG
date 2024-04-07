#pragma once
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayTagContainer.h"
#include "ICombat.generated.h"

class UAnimMontage;
class UNiagaraSystem;

//** Attack Montage와 해당 몽타주에 대응하는 GameplayTag을 같이 사용하기 위해 구조체를 만들어 사용*/
USTRUCT(BlueprintType)
struct FTaggedMontage
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* Montage = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag MontageTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag SocketTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<USoundBase> ImpactSound = nullptr;
};

UINTERFACE(MinimalAPI, BlueprintType)
class UICombat : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TDRPG_API IICombat
{
	GENERATED_BODY()

public:
	virtual int32 GetPlayerLevel();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FVector GetCombatSocketLocation(const FGameplayTag& MontageTag); // 소켓 위치를 리턴

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable) // BP ver.
	void UpdateFacingTarget(const FVector& Target);
	UFUNCTION() // C++ ver.
	virtual void SetFacingTarget(const FVector& FacingTarget);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UAnimMontage* GetHitReactMontage();
	virtual UAnimMontage* GetHitReactMontageCPP();

	virtual void Die();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsDead() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	AActor* GetAvatar();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetCombatTarget(AActor* InCombatTarget);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	AActor* GetCombatTarget() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	TArray<FTaggedMontage> GetAttackMontages();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UNiagaraSystem* GetBloodEffect();
	virtual UNiagaraSystem* GetBloodEffectCPP();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FTaggedMontage GetTaggedMontageByTag(const FGameplayTag& MontageTag);
	virtual FTaggedMontage GetTaggedMontageByTagCPP(const FGameplayTag& MontageTag);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int32 GetMinionCount();
	virtual int32 GetMinionCountCPP();


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void IncremenetMinionCount(int32 Amount);
	virtual void IncremenetMinionCntCPP(int32 Amount);

};
