#pragma once
#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/Character.h"
#include "GAS/Data/TDDA_CharacterClass.h"
#include "Interface/ICombat.h"
#include "TDBaseCharacter.generated.h"

class UMotionWarpingComponent;
class UAbilitySystemComponent;
class UTDDebuffComponent;
class UAttributeSet;
class UGameplayEffect;
class UGameplayAbility;
class UAnimMontage;
class UNiagaraSystem;

UCLASS(Abstract)
class TDRPG_API ATDBaseCharacter : public ACharacter, public IAbilitySystemInterface, public IICombat, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	ATDBaseCharacter();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	//** ICombat *********************************************

	virtual void UpdateFacingTargetCPP(const FVector& FacingTarget) override;
	virtual UAnimMontage* GetHitReactMontage_Implementation() override;
	virtual UAnimMontage* GetHitReactMontageCPP() override;
	virtual void Die(const FVector& RagdollImpulse) override;
	virtual FVector GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag) override;
	virtual FVector GetCombatSocketLocationCPP(const FGameplayTag& MontageTag) override;
	virtual bool IsDeadBP_Implementation() const override;
	virtual bool IsDead() const override;
	virtual AActor* GetAvatar_Implementation() override;
	virtual TArray<FTaggedMontage> GetAttackMontages_Implementation() override;
	virtual TArray<FTaggedMontage> GetAttackMontagesCPP() override;
	virtual UNiagaraSystem* GetBloodEffect_Implementation() override;
	virtual UNiagaraSystem* GetBloodEffectCPP() override;
	virtual FTaggedMontage GetTaggedMontageByTag_Implementation(const FGameplayTag& MontageTag) override;
	virtual FTaggedMontage GetTaggedMontageByTagCPP(const FGameplayTag& MontageTag) override;
	virtual int32 GetMinionCount_Implementation() override;
	virtual void IncremenetMinionCount_Implementation(int32 Amount) override;
	virtual void IncremenetMinionCntCPP(int32 Amount) override;
	virtual int32 GetMinionCountCPP() override;
	virtual ECharacterClass GetCharacterClass_Implementation() override;
	virtual ECharacterClass GetCharacterClassCPP() override;
	virtual FOnASCRegisteredSignature GetOnASCRegisteredDelegate() override;
	virtual FOnDeathSignature GetOnDeathDelegate() override;
	virtual void SetInShockLoop(bool bInLoop) override;
	virtual USkeletalMeshComponent* GetWeaponBP_Implementation() override;
	virtual USkeletalMeshComponent* GetWeapon() override;
	//********************************************************

	UFUNCTION(NetMulticast, Reliable)
	virtual void Multicast_ApplyDeath(const FVector& RagdollImpulse); // 캐릭터 사망 처리

	UPROPERTY(EditDefaultsOnly, Category = "Character Class Defaults")
	TObjectPtr<UTDDA_CharacterClass> TDDACharacterClass;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TArray<FTaggedMontage> ATKMontages; // 공격 몽타주 배열

	FOnASCRegisteredSignature OnASCRegisteredDelegate;
	FOnDeathSignature OnDeathDelegate;

protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo();
	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const;
	virtual void InitializeDefaultAttributes() const; // Attributes 초기값 설정
	void AddCharacterAbilities();

	UPROPERTY(EditAnywhere, Category = "Combat| Team")
	uint8 TeamID;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	UPROPERTY(EditAnywhere, Category = "Combat") // 에디터에서 적음
	FName WeaponTipSocketName; // 무기 소켓 이름
	UPROPERTY(EditAnywhere, Category = "Combat")
	FName HeadSocketName;
	UPROPERTY(EditAnywhere, Category = "Combat")
	FName LeftHandSocketName;
	UPROPERTY(EditAnywhere, Category = "Combat")
	FName RightHandSocketName;
	UPROPERTY(EditAnywhere, Category = "Combat")
	FName TrailSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<UNiagaraSystem> BloodEffect;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<USoundBase> DeathSound;

	bool bDead = false;


	UPROPERTY(EditAnywhere, Category = "Motion Warping Component")
	FName WarpTargetName = FName("FacingTarget");

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Motion Warping Component")
	TObjectPtr<UMotionWarpingComponent> MotionWarpingComponent;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, Category = "Debuff Component")
	TObjectPtr<UTDDebuffComponent> DebuffComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultStatAttributes;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;

	int32 MinionCnt = 0; // 미니언

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;

private:
	UPROPERTY(EditAnywhere, Category = "GameplayAbility")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities; // GameplayAbility를 에디터에서 모두 할당
	UPROPERTY(EditAnywhere, Category = "GameplayAbility")
	TArray<TSubclassOf<UGameplayAbility>> StartupPassiveAbilities;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;
};
