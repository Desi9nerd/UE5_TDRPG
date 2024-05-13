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

	FGameplayTag Debuff_DotDamage;
	FGameplayTag Debuff_Slow;
	FGameplayTag Debuff_Stun;
	FGameplayTag Debuff_None;

	FGameplayTag Debuff_Chance;
	FGameplayTag Debuff_Damage;
	FGameplayTag Debuff_Duration;
	FGameplayTag Debuff_Frequency;

	// Meta Attributes
	FGameplayTag Attributes_Meta_IncomingExp;

	// Abilities
	FGameplayTag Abilities_Attack;
	FGameplayTag Abilities_Summon;
	FGameplayTag Abilities_HitReact;
	FGameplayTag Abilities_None;

	FGameplayTag Abilities_Type_Active;
	FGameplayTag Abilities_Type_Passive;
	FGameplayTag Abilities_Type_None;

	FGameplayTag Abilities_Status_Locked;
	FGameplayTag Abilities_Status_Eligible;
	FGameplayTag Abilities_Status_Unlocked;
	FGameplayTag Abilities_Status_Equipped;


	FGameplayTag Abilities_Skill_Fireball;
	FGameplayTag Abilities_Skill_MagicArrow;
	FGameplayTag Abilities_Skill_ComboFlameShooting;
	FGameplayTag Abilities_Skill_EnergyLance;
	FGameplayTag Abilities_Skill_StoneEdge;
	FGameplayTag Abilities_Skill_Meteor;

	FGameplayTag Abilities_Skill_Flash;
	FGameplayTag Abilities_Skill_Dash;

	// Cooldown
	FGameplayTag Cooldown_Skill_Fireball;
	FGameplayTag Cooldown_Skill_MagicArrow;
	FGameplayTag Cooldown_Skill_ComboFlameShooting;
	FGameplayTag Cooldown_Skill_EnergyLance;
	FGameplayTag Cooldown_Skill_StoneEdge;
	FGameplayTag Cooldown_Skill_Meteor;
	FGameplayTag Cooldown_Skill_Flash;
	FGameplayTag Cooldown_Skill_Dash;

	//** Meta ************************************************
	// Damage Type
	FGameplayTag DamageType;
	FGameplayTag DamageType_Fireball;
	FGameplayTag DamageType_Meteor;
	FGameplayTag DamageType_Ice;

	FGameplayTag DamageType_Melee;
	FGameplayTag DamageType_None;

	// TMap. key: DamageType, value: Resistance
	TMap<FGameplayTag, FGameplayTag> DamageTypesToResistances;
	TMap<FGameplayTag, FGameplayTag> DamageTypesToDebuffs;

	// Effect
	FGameplayTag Effect_HitReact;

	// Event
	FGameplayTag Event_Montage_Fireball;
	FGameplayTag Event_Montage_MagicArrow;
	FGameplayTag Event_Montage_ComboFlameShooting;
	FGameplayTag Event_Montage_EnergyLance;
	FGameplayTag Event_Montage_StoneEdge;
	FGameplayTag Event_Montage_Meteor;
	FGameplayTag Event_Montage_Flash;
	FGameplayTag Event_Montage_Dash;

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

	//********************************************************
	// Enhanced Input
	FGameplayTag InputTag_Q;
	FGameplayTag InputTag_W;
	FGameplayTag InputTag_E;
	FGameplayTag InputTag_R;
	FGameplayTag InputTag_A;
	FGameplayTag InputTag_S;
	FGameplayTag InputTag_D;
	FGameplayTag InputTag_F;
	FGameplayTag InputTag_V;
	FGameplayTag InputTag_LMB;
	FGameplayTag InputTag_RMB;
	FGameplayTag InputTag_Spacebar;
	FGameplayTag InputTag_Passive_1;
	FGameplayTag InputTag_Passive_2;

	// Input 상태: Pressed, Held, Released, CursorTrace
	FGameplayTag BlockTag_InputPressed;
	FGameplayTag BlockTag_InputHeld;
	FGameplayTag BlockTag_InputReleased;
	FGameplayTag BlockTag_CursorTrace;
	//********************************************************

	// Gameplay Cues
	FGameplayTag GameplayCue_MeleeImpact;

	//********************************************************
	// Inventory + Item
	FGameplayTag Inventory_Open;
	FGameplayTag Item_Equip;
	FGameplayTag Item_EquipNext;
	FGameplayTag Item_Unequip;
	FGameplayTag Item_Drop;
	//********************************************************

private:
	static FTDGameplayTags GameplayTags;
};