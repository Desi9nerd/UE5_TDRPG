#pragma once
#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h" // #define ATTRIBUTE_ACCESSORS 매크로에 사용
#include "TDAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

// DECLARE_DELEGATE_RetVal(FGameplayAttribute, FAttributeSignature);를 아래와 같이 수정
// typedef는 FGameplayAttribute() signature에 한정해서 사용한다. 반면에 아래와 같이 TStaticFunPtr로 수정하면 모든 signature에 사용 가능하다.
// typedef TBaseStaticDelegateInstance<FGameplayAttribute(), FDefaultDelegateUserPolicy>::FFuncPtr FAttributeFuncPtr;를 template 형태로 변형하여 다형성을 확보하였다.
template<class T>
using TStaticFuncPtr = typename TBaseStaticDelegateInstance<T, FDefaultDelegateUserPolicy>::FFuncPtr;

/**
 * 
 */

USTRUCT()
struct FEffectProperties // 이펙트 속성 구조체
{
	GENERATED_BODY()

	FEffectProperties() {}

	FGameplayEffectContextHandle EffectContextHandle;

	UPROPERTY()
	UAbilitySystemComponent* SourceASC = nullptr;
	UPROPERTY()
	AActor* SourceAvatarActor = nullptr;
	UPROPERTY()
	AController* SourceController = nullptr;
	UPROPERTY()
	ACharacter* SourceCharacter = nullptr;

	UPROPERTY()
	UAbilitySystemComponent* TargetASC = nullptr;
	UPROPERTY()
	AActor* TargetAvatarActor = nullptr;
	UPROPERTY()
	AController* TargetController = nullptr;
	UPROPERTY()
	ACharacter* TargetCharacter = nullptr;
};

UCLASS()
class TDRPG_API UTDAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UTDAttributeSet();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;


	TMap<FGameplayTag, TStaticFuncPtr<FGameplayAttribute()>> TagsToAttributes;

	ATTRIBUTE_ACCESSORS(UTDAttributeSet, Strength);
	ATTRIBUTE_ACCESSORS(UTDAttributeSet, Intelligence);
	ATTRIBUTE_ACCESSORS(UTDAttributeSet, Resilience);
	ATTRIBUTE_ACCESSORS(UTDAttributeSet, Vigor);
	ATTRIBUTE_ACCESSORS(UTDAttributeSet, Armor);
	ATTRIBUTE_ACCESSORS(UTDAttributeSet, ArmorPenetration);
	ATTRIBUTE_ACCESSORS(UTDAttributeSet, BlockChance);
	ATTRIBUTE_ACCESSORS(UTDAttributeSet, CriticalHitChance);
	ATTRIBUTE_ACCESSORS(UTDAttributeSet, CriticalHitResistance);
	ATTRIBUTE_ACCESSORS(UTDAttributeSet, CriticalHitDamage);
	ATTRIBUTE_ACCESSORS(UTDAttributeSet, HealthRegeneration);
	ATTRIBUTE_ACCESSORS(UTDAttributeSet, ManaRegeneration);
	ATTRIBUTE_ACCESSORS(UTDAttributeSet, MaxHealth);
	ATTRIBUTE_ACCESSORS(UTDAttributeSet, MaxMana);
	ATTRIBUTE_ACCESSORS(UTDAttributeSet, Health);
	ATTRIBUTE_ACCESSORS(UTDAttributeSet, Mana);
	ATTRIBUTE_ACCESSORS(UTDAttributeSet, Soul);
	ATTRIBUTE_ACCESSORS(UTDAttributeSet, FireballResistance);
	ATTRIBUTE_ACCESSORS(UTDAttributeSet, MeteorResistance);
	ATTRIBUTE_ACCESSORS(UTDAttributeSet, MeleeResistance);
	ATTRIBUTE_ACCESSORS(UTDAttributeSet, IncomingDamage);
	ATTRIBUTE_ACCESSORS(UTDAttributeSet, IncomingExp);
	ATTRIBUTE_ACCESSORS(UTDAttributeSet, HitReact);

	
	//** Stat Attributes
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Strength, Category = "Stat Attributes")
	FGameplayAttributeData Strength;
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Intelligence, Category = "Stat Attributes")
	FGameplayAttributeData Intelligence;
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Resilience, Category = "Stat Attributes")
	FGameplayAttributeData Resilience;
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Vigor, Category = "Stat Attributes")
	FGameplayAttributeData Vigor;

	//** Secondary Attributes
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Armor, Category = "Secondary Attributes")
	FGameplayAttributeData Armor;
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ArmorPenetration, Category = "Secondary Attributes")
	FGameplayAttributeData ArmorPenetration;
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_BlockChance, Category = "Secondary Attributes")
	FGameplayAttributeData BlockChance;
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitChance, Category = "Secondary Attributes")
	FGameplayAttributeData CriticalHitChance;
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitDamage, Category = "Secondary Attributes")
	FGameplayAttributeData CriticalHitDamage;
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitResistance, Category = "Secondary Attributes")
	FGameplayAttributeData CriticalHitResistance;
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_HealthRegeneration, Category = "Secondary Attributes")
	FGameplayAttributeData HealthRegeneration;
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ManaRegeneration, Category = "Secondary Attributes")
	FGameplayAttributeData ManaRegeneration;
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Vital Attributes")
	FGameplayAttributeData MaxHealth;
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana, Category = "Vital Attributes")
	FGameplayAttributeData MaxMana;

	//** Vital Attributes
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Vital Attributes")
	FGameplayAttributeData Health;
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana, Category = "Vital Attributes")
	FGameplayAttributeData Mana;
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Soul, Category = "Vital Attributes")
	FGameplayAttributeData Soul;

	//** Resistance Attributes
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_FireballResistance, Category = "Resistance Attributes")
	FGameplayAttributeData FireballResistance;
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MeteorResistance, Category = "Resistance Attributes")
	FGameplayAttributeData MeteorResistance;
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MeleeResistance, Category = "Resistance Attributes")
	FGameplayAttributeData MeleeResistance;


	//** Meta Attributes (Attribute와 상호 작용할 임시 값. Replicated X)
	UPROPERTY(BlueprintReadOnly, Category = "Meta Attributes")
	FGameplayAttributeData IncomingDamage;
	UPROPERTY(BlueprintReadOnly, Category = "Meta Attributes")
	FGameplayAttributeData IncomingExp;
	UPROPERTY(BlueprintReadOnly, Category = "Meta Attributes")
	FGameplayAttributeData HitReact;


	//** Stat Attibutes
	UFUNCTION()
	void OnRep_Strength(const FGameplayAttributeData& OldStrength) const;
	UFUNCTION()
	void OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const;
	UFUNCTION()
	void OnRep_Resilience(const FGameplayAttributeData& OldResilience) const;
	UFUNCTION()
	void OnRep_Vigor(const FGameplayAttributeData& OldVigor) const;
	//** Secondary Attributes
	UFUNCTION()
	void OnRep_Armor(const FGameplayAttributeData& OldArmor) const;
	UFUNCTION()
	void OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) const;
	UFUNCTION()
	void OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const;
	UFUNCTION()
	void OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const;
	UFUNCTION()
	void OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage) const;
	UFUNCTION()
	void OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance) const;
	UFUNCTION()
	void OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const;
	UFUNCTION()
	void OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const;
	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;
	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;
	//** Vital Attributes
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;
	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana) const;
	UFUNCTION()
	void OnRep_Soul(const FGameplayAttributeData& OldSoul) const;

	//** Resistance Attributes
	UFUNCTION()
	void OnRep_FireballResistance(const FGameplayAttributeData& OldFireballResistance) const;
	UFUNCTION()
	void OnRep_MeteorResistance(const FGameplayAttributeData& OldMeteorResistance) const;
	UFUNCTION()
	void OnRep_MeleeResistance(const FGameplayAttributeData& OldMeleeResistance) const;

private:
	void SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const; // 이펙트 속성 설정하기
	void ShowFloatingText(const FEffectProperties& Props, float Damage, bool bBlockedHit, bool bCriticalHit) const;
	void SendExpEvent(const FEffectProperties& Props);

	bool bMakeMaxHealth = false;
	bool bMakeMaxMana = false;
};
