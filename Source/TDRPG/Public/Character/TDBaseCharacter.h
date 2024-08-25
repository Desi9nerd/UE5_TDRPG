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

//=== Get, Set func ===========================================================
public:
	TObjectPtr<UTDAbilitySystemComponent> GetTDASC();
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }
	virtual FOnDamageSignature& GetOnDamageSignature() override { return OnDamageDelegate; };
	FORCEINLINE void SetCharacterClass(ECharacterClass InClass) { CharacterClass = InClass; }
	
protected:
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
	
private:
	UPROPERTY()
	TObjectPtr<UTDAbilitySystemComponent> TDASC;
//=== Get, Set func ===========================================================

//=== IAbilitySystemInterface =================================================
public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
//=== IAbilitySystemInterface =================================================

//=== ICombat =================================================================
public:
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
//=== ICombat =================================================================
	
	UFUNCTION(NetMulticast, Reliable)
	virtual void Multicast_ApplyDeath(const FVector& RagdollImpulse); // 캐릭터 사망 처리

	UPROPERTY(EditDefaultsOnly, Category = "Character Class Defaults")
	TObjectPtr<UTDDA_CharacterClass> TDDACharacterClass;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TArray<FTaggedMontage> ATKMontages; // 공격 몽타주 배열

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
	virtual void InitializeDefaultAttributes() const; // Attributes 초기값 설정
	void AddCharacterAbilities();
	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;


	UPROPERTY(EditAnywhere, Category = "Combat| Team")
	uint8 TeamID;

	//********************************************************
	//** 소켓
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Socket") // 에디터에서 적음
	FName WeaponTipSocketName = FName(TEXT("TipSocket"));
	UPROPERTY(EditAnywhere, Category = "Socket")
	FName HeadSocketName = FName(TEXT("HeadSocket"));
	UPROPERTY(EditAnywhere, Category = "Socket")
	FName LeftHandSocketName = FName(TEXT("LeftHandSocket"));
	UPROPERTY(EditAnywhere, Category = "Socket")
	FName RightHandSocketName = FName(TEXT("RightHandSocket"));
	UPROPERTY(EditAnywhere, Category = "Socket")
	FName LeftLegSocketName = FName(TEXT("LeftLegSocket"));
	UPROPERTY(EditAnywhere, Category = "Socket")
	FName RightLegSocketName = FName(TEXT("RightLegSocket"));
	UPROPERTY(EditAnywhere, Category = "Socket")
	FName TrailSocketName;
	//********************************************************

	//********************************************************
	//** 매쉬, 이펙트, 사운드
	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<UNiagaraSystem> BloodEffect;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<USoundBase> DeathSound;
	//********************************************************

	//********************************************************
	//** 캐릭터 상태
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float BaseWalkSpeed = 300.f;

	UPROPERTY(BlueprintReadOnly)
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

	UPROPERTY(VisibleAnywhere, Category = "Debuff Component")
	TObjectPtr<UTDDebuffComponent> DotDamageDebuffComponent;
	UPROPERTY(VisibleAnywhere, Category = "Debuff Component")
	TObjectPtr<UTDDebuffComponent> StunDebuffComponent;
	//********************************************************

	//********************************************************
	//** Attributes
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultStatAttributes;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;
	//********************************************************

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
