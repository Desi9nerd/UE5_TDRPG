#pragma once
#include "CoreMinimal.h"
#include "GameplayTagContainer.h" // GameplayTag 타입 정보

/** native GamplayTags를 가지는 Singleton
 *  코드로 에디터에 TDGameplayTags 등록
 */
struct FTDGameplayTags
{
public:
	static const FTDGameplayTags& GetTDGameplayTags() { return GameplayTags; }
	static void InitializeNativeGameplayTags(); // GameplayTags 등록

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

	// Resistance
	FGameplayTag Attributes_Resistance_Fireball;
	FGameplayTag Attributes_Resistance_Meteor;

	FGameplayTag Attributes_Resistance_Melee;

	// Abilities
	FGameplayTag Abilities_Attack;
	FGameplayTag Abilities_Summon;

	//** Meta ************************************************
	// Damage Type
	FGameplayTag Damage;
	FGameplayTag Damage_Fireball;
	FGameplayTag Damage_Meteor;

	FGameplayTag Damage_Melee;

	// TMap. key: DamageType, value: Resistance
	TMap<FGameplayTag, FGameplayTag> DamageTypesToResistances;

	// Effect
	FGameplayTag Effect_HitReact;

	// Event
	FGameplayTag Event_Montage_Fireball;

	// Socket
	FGameplayTag Socket_Weapon;
	FGameplayTag Socket_Head;
	FGameplayTag Socket_RightHand;
	FGameplayTag Socket_LeftHand;
	FGameplayTag Socket_Trail;

	// Montage
	FGameplayTag Montage_ATK_1;
	FGameplayTag Montage_ATK_2;
	FGameplayTag Montage_ATK_3;
	FGameplayTag Montage_ATK_4;

	// Enhanced Input
	FGameplayTag InputTag_LMB;
	FGameplayTag InputTag_RMB;
	FGameplayTag InputTag_Q;
	FGameplayTag InputTag_W;
	FGameplayTag InputTag_E;
	FGameplayTag InputTag_R;
	//********************************************************

	// Gameplay Cues
	FGameplayTag GameplayCue_MeleeImpact;

private:
	static FTDGameplayTags GameplayTags;
};