#pragma once
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayTagContainer.h"
#include "GAS/Data/TDDA_CharacterClass.h"
#include "ICombat.generated.h"

class UAnimMontage;
class UNiagaraSystem;
class UAbilitySystemComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnASCRegisteredSignature, UAbilitySystemComponent*);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeathSignature, AActor*, DeadActor);

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
	UFUNCTION(BlueprintNativeEvent)
	int32 GetPlayerLevelBP();
	virtual int32 GetPlayerLevel();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FVector GetCombatSocketLocation(const FGameplayTag& MontageTag); // 소켓 위치를 리턴
	virtual FVector GetCombatSocketLocationCPP(const FGameplayTag& MontageTag) = 0; // 소켓 위치를 리턴

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void UpdateFacingTarget(const FVector& Target);
	virtual void UpdateFacingTargetCPP(const FVector& FacingTarget) = 0;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UAnimMontage* GetHitReactMontage();
	virtual UAnimMontage* GetHitReactMontageCPP() = 0;

	virtual void Die(const FVector& RagdollImpulse) = 0;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsDeadBP() const;
	virtual bool IsDead() const = 0;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	AActor* GetAvatar();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetCombatTarget(AActor* InCombatTarget);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	AActor* GetCombatTarget() const;
	virtual AActor* GetCombatTargetCPP() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	TArray<FTaggedMontage> GetAttackMontages();
	virtual TArray<FTaggedMontage> GetAttackMontagesCPP() = 0;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UNiagaraSystem* GetBloodEffect();
	virtual UNiagaraSystem* GetBloodEffectCPP() = 0;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FTaggedMontage GetTaggedMontageByTag(const FGameplayTag& MontageTag);
	virtual FTaggedMontage GetTaggedMontageByTagCPP(const FGameplayTag& MontageTag) = 0;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int32 GetMinionCount();
	virtual int32 GetMinionCountCPP() = 0;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void IncremenetMinionCount(int32 Amount);
	virtual void IncremenetMinionCntCPP(int32 Amount) = 0;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	ECharacterClass GetCharacterClass();
	virtual ECharacterClass GetCharacterClassCPP() = 0;

	virtual FOnASCRegisteredSignature GetOnASCRegisteredDelegate() = 0;
	virtual FOnDeathSignature GetOnDeathDelegate() = 0;
};
