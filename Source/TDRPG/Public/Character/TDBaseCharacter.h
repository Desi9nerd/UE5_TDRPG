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
class UTDAbilitySystemComponent;
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
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	TObjectPtr<UTDAbilitySystemComponent> GetTDASC();
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }
	virtual FOnDamageSignature& GetOnDamageSignature() override { return OnDamageDelegate; };

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
	virtual FOnDeathSignature& GetOnDeathDelegate() override;
	virtual void SetInShockLoop(bool bInLoop) override;
	virtual void SetBeingShockedBP_Implementation(bool bInShock) override;
	virtual void SetBeingShocked(bool bInShock) override;
	virtual bool BeingShockedBP_Implementation() const override;
	virtual bool BeingShocked() const override;
	virtual USkeletalMeshComponent* GetWeaponBP_Implementation() override;
	virtual USkeletalMeshComponent* GetWeapon() override;
	virtual void SpawnWeapon() override;
	//********************************************************

	UFUNCTION(NetMulticast, Reliable)
	virtual void Multicast_ApplyDeath(const FVector& RagdollImpulse); // ĳ���� ��� ó��

	UPROPERTY(EditDefaultsOnly, Category = "Character Class Defaults")
	TObjectPtr<UTDDA_CharacterClass> TDDACharacterClass;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TArray<FTaggedMontage> ATKMontages; // ���� ��Ÿ�� �迭

	//********************************************************
	//** Delegate
	FOnASCRegisteredSignature OnASCRegisteredDelegate;
	FOnDamageSignature OnDamageDelegate;
	FOnDeathSignature OnDeathDelegate;
	//********************************************************

protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo();
	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const;
	virtual void InitializeDefaultAttributes() const; // Attributes �ʱⰪ ����
	void AddCharacterAbilities();
	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;


	UPROPERTY(EditAnywhere, Category = "Combat| Team")
	uint8 TeamID;

	//********************************************************
	//** ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat") // �����Ϳ��� ����
	FName WeaponTipSocketName; // ���� ���� �̸�
	UPROPERTY(EditAnywhere, Category = "Combat")
	FName HeadSocketName;
	UPROPERTY(EditAnywhere, Category = "Combat")
	FName LeftHandSocketName;
	UPROPERTY(EditAnywhere, Category = "Combat")
	FName RightHandSocketName;
	UPROPERTY(EditAnywhere, Category = "Combat")
	FName TrailSocketName;
	//********************************************************

	//********************************************************
	//** �Ž�, ����Ʈ, ����
	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<UNiagaraSystem> BloodEffect;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<USoundBase> DeathSound;
	//********************************************************

	//********************************************************
	//** ĳ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float BaseWalkSpeed = 300.f;

	bool bDead = false;

	UPROPERTY(Replicated, BlueprintReadOnly)
	bool bBeingShocked = false;
	UPROPERTY(ReplicatedUsing = OnRep_Stunned, BlueprintReadOnly)
	bool bStunned = false;
	UPROPERTY(ReplicatedUsing = OnRep_Burned, BlueprintReadOnly)
	bool bBurned = false;

	UFUNCTION()
	virtual void OnRep_Stunned();
	UFUNCTION()
	virtual void OnRep_Burned();
	//********************************************************

	UPROPERTY(EditAnywhere, Category = "Motion Warping Component")
	FName WarpTargetName = FName("FacingTarget");

	//********************************************************
	//** Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Motion Warping Component")
	TObjectPtr<UMotionWarpingComponent> MotionWarpingComponent;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, Category = "Debuff Component")
	TObjectPtr<UTDDebuffComponent> DebuffComponent;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UTDDebuffComponent> StunDebuffComponent;
	//********************************************************

	//********************************************************
	//** Attributes
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultStatAttributes;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;
	//********************************************************

	int32 MinionCnt = 0; // �̴Ͼ�

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;

private:
	UPROPERTY(EditAnywhere, Category = "GameplayAbility")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities; // GameplayAbility�� �����Ϳ��� ��� �Ҵ�
	UPROPERTY(EditAnywhere, Category = "GameplayAbility")
	TArray<TSubclassOf<UGameplayAbility>> StartupPassiveAbilities;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;

	UPROPERTY()
	TObjectPtr<UTDAbilitySystemComponent> TDASC;
};
