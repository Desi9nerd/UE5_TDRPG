#pragma once
#include "CoreMinimal.h"
#include "GameplayTagContainer.h" // GameplayTag Ÿ�� ����

/** native GamplayTags�� ������ Singleton
 *  �ڵ�� �����Ϳ� TDGameplayTags ���
 */
struct FTDGameplayTags
{
public:
	static const FTDGameplayTags& GetTDGameplayTags() { return GameplayTags; }
	static void InitializeNativeGameplayTags(); // GameplayTags ���

	// Stat Attributes
	FGameplayTag Attributes_Stat_Strength;
	FGameplayTag Attributes_Stat_Intelligence;
	FGameplayTag Attributes_Stat_Resilience;
	FGameplayTag Attributes_Stat_Vigor;
	// Secondary Attributes
	FGameplayTag Attributes_Secondary_Armor;
	FGameplayTag Attributes_Secondary_ArmorPenetration;
	FGameplayTag Attributes_Secondary_BlockChance;
	FGameplayTag Attributes_Secondary_CriticalHitChance;
	FGameplayTag Attributes_Secondary_CriticalHitDamage;
	FGameplayTag Attributes_Secondary_CriticalHitResistance;
	FGameplayTag Attributes_Secondary_HealthRegeneration;
	FGameplayTag Attributes_Secondary_ManaRegeneration;
	FGameplayTag Attributes_Secondary_MaxHealth;
	FGameplayTag Attributes_Secondary_MaxMana;


	//** Meta
	// Damage Type
	FGameplayTag Damage;
	FGameplayTag Damage_Fireball;
	FGameplayTag Damage_Meteor;

	// Resistance
	FGameplayTag Attributes_Resistance_Fireball;
	FGameplayTag Attributes_Resistance_Meteor;

	// TMap. key: DamageType, value: Resistance
	TMap<FGameplayTag, FGameplayTag> DamageTypesToResistances;

	// Effect
	FGameplayTag Effect_HitReact;

	// Event
	FGameplayTag Event_Montage_Fireball;

	// Enhanced Input
	FGameplayTag InputTag_LMB;
	FGameplayTag InputTag_RMB;
	FGameplayTag InputTag_Q;
	FGameplayTag InputTag_W;
	FGameplayTag InputTag_E;
	FGameplayTag InputTag_R;

private:
	static FTDGameplayTags GameplayTags;
};