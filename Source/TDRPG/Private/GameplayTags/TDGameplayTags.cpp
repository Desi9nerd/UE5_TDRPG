#include "GameplayTags/TDGameplayTags.h"
#include "GameplayTagsManager.h"

FTDGameplayTags FTDGameplayTags::GameplayTags; // static GamplayTags instance

void FTDGameplayTags::InitializeNativeGameplayTags()
{
	// Stat Attributes
	GameplayTags.Attributes_Stat_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Stat.Strength"),FString("물리 데미지 증가"));
	GameplayTags.Attributes_Stat_Intelligence = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Stat.Intelligence"),FString("마법 데미지 증가"));
	GameplayTags.Attributes_Stat_Resilience = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Stat.Resilience"),FString("방어력과 침투방어력 증가"));
	GameplayTags.Attributes_Stat_Vigor = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Stat.Vigor"),FString("체력 증가"));

	// Secondary Attributes
	GameplayTags.Attributes_Secondary_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.Armor"),FString("데미지 피해 감소, Block chance 증가"));
	GameplayTags.Attributes_Secondary_ArmorPenetration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.ArmorPenetration"),FString("적의 방어확률 무시, 크리티컬히트 확률 증가"));
	GameplayTags.Attributes_Secondary_BlockChance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.BlockChance"),FString("데미지 피해 절반 감소 확률 증가"));
	GameplayTags.Attributes_Secondary_CriticalHitChance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.CriticalHitChance"),FString("크리티컬 히트 피해 확률 증가"));
	GameplayTags.Attributes_Secondary_CriticalHitDamage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.CriticalHitDamage"), FString("크리티컬 히트 데미지"));
	GameplayTags.Attributes_Secondary_CriticalHitResistance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.CriticalHitResistance"), FString("상대방의 크리티컬 히트 공격을 받을 확률 감소"));
	GameplayTags.Attributes_Secondary_HealthRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.HealthRegeneration"), FString("매초 회복되는 체력량"));
	GameplayTags.Attributes_Secondary_ManaRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.ManaRegeneration"), FString("매초 회복되는 마나량"));
	GameplayTags.Attributes_Secondary_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.MaxHealth"), FString("총 체력량"));
	GameplayTags.Attributes_Secondary_MaxMana = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.MaxMana"), FString("총 마나량"));

	// Abilities
	GameplayTags.Abilities_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Attack"), FString("Abilities: 공격"));
	GameplayTags.Abilities_Summon = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Summon"), FString("Abilities: 미니언 소환"));
	GameplayTags.Abilities_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.HitReact"), FString("Abilities: 피격됨"));

	GameplayTags.Abilities_Type_Active = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Type.Active"), FString("Abilities 타입: 액티브 스킬"));
	GameplayTags.Abilities_Type_Passive = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Type.Passive"), FString("Abilities 타입: 패시브 스킬"));
	GameplayTags.Abilities_Type_None = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Type.None"), FString("Abilities 타입: None"));

	GameplayTags.Abilities_Status_Locked = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Status.Locked"), FString("Abilities 상태: 잠금"));
	GameplayTags.Abilities_Status_Eligible = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Status.Eligible"), FString("Abilities 상태: 사용가능"));
	GameplayTags.Abilities_Status_Unlocked = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Status.Unlocked"), FString("Abilities 상태: 해금됨"));
	GameplayTags.Abilities_Status_Equipped = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Status.Equipped"), FString("Abilities 상태: 장착됨"));

	GameplayTags.Abilities_Skill_Fireball = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Skill.Fireball"), FString("Abilities: 파이어볼"));
	GameplayTags.Abilities_Skill_Meteor = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Skill.Meteor"), FString("Abilities: 메테오"));

	// Cooldown
	GameplayTags.Cooldown_Skill_Fireball = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Skill.Fireball"), FString("Cooldown: 파이어볼"));

	//** Meta
	// Damage Type
	GameplayTags.Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage"), FString("데미지"));
	GameplayTags.Damage_Fireball = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Fireball"), FString("파이어볼 데미지"));
	GameplayTags.Damage_Meteor = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Meteor"), FString("메테오 데미지"));
	GameplayTags.Damage_Melee = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Melee"), FString("근접공격 데미지"));

	// Resistance
	GameplayTags.Attributes_Resistance_Fireball = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Fireball"), FString("파이어볼 데미지 저항"));
	GameplayTags.Attributes_Resistance_Meteor = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Meteor"), FString("메테오 데미지 저항"));
	GameplayTags.Attributes_Resistance_Melee = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Melee"), FString("근접공격 데미지 저항"));

	// Meta Attributes
	GameplayTags.Attributes_Meta_IncomingExp = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Meta.IncomingExp"), FString("경험치 획득 Meta Attribute"));

	// TMap 등록. DamageTypesToResistances
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Fireball, GameplayTags.Attributes_Resistance_Fireball);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Meteor, GameplayTags.Attributes_Resistance_Meteor);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Melee, GameplayTags.Attributes_Resistance_Melee);

	// Effect
	GameplayTags.Effect_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Effect.HitReact"), FString("피격 시 GameplayTag Granted 반응"));

	// Event
	GameplayTags.Event_Montage_Fireball = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Event.Montage.Fireball"), FString("파이어볼"));

	// Socket
	GameplayTags.Socket_Weapon = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Socket.Weapon"),FString("무기 소켓"));
	GameplayTags.Socket_Head = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Socket.Head"), FString("머리(대가리) 소켓"));
	GameplayTags.Socket_RightHand = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Socket.RightHand"),FString("오른손 소켓"));
	GameplayTags.Socket_LeftHand = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Socket.LeftHand"),FString("왼손 소켓"));
	GameplayTags.Socket_Trail = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Socket.Trail"),FString("공격 시 무기/손 트레일이펙트 위치"));

	// Montage
	GameplayTags.Montage_ATK_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.ATK.1"), FString("공격 1"));
	GameplayTags.Montage_ATK_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.ATK.2"), FString("공격 2"));
	GameplayTags.Montage_ATK_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.ATK.3"), FString("공격 3"));
	GameplayTags.Montage_ATK_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.ATK.4"), FString("공격 4"));

	// Enhanced Input
	GameplayTags.InputTag_Q = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.Q"), FString("Q버튼 Input Tag"));
	GameplayTags.InputTag_W = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.W"), FString("W버튼 Input Tag"));
	GameplayTags.InputTag_E = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.E"),FString("E버튼 Input Tag"));
	GameplayTags.InputTag_R = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.R"),FString("R버튼 Input Tag"));
	GameplayTags.InputTag_A = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.A"),FString("A버튼 Input Tag"));
	GameplayTags.InputTag_S = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.S"),FString("S버튼 Input Tag"));
	GameplayTags.InputTag_D = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.D"),FString("D버튼 Input Tag"));
	GameplayTags.InputTag_F = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.F"),FString("F버튼 Input Tag"));
	GameplayTags.InputTag_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.LMB"), FString("왼쪽 마우스 버튼 Input Tag"));
	GameplayTags.InputTag_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.RMB"), FString("오른쪽 마우스 버튼 Input Tag"));
	GameplayTags.InputTag_Spacebar = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.Spacebar"), FString("스페이스바 버튼 Input Tag"));
	GameplayTags.InputTag_Passive_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.Passive.1"),FString("패시브1 Input Tag"));
	GameplayTags.InputTag_Passive_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.Passive.2"),FString("패시브2 Input Tag"));

	// Gameplay Cues
	GameplayTags.GameplayCue_MeleeImpact = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.MeleeImpact"), FString("MeleeAttack 피격 시 이펙트"));

	// Inventory + Item
	GameplayTags.Item_Equip = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Item.Equip"), FString("아이템 장착"));
	GameplayTags.Item_EquipNext = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Item.EquipNext"), FString("다음 아이템 장착"));
	GameplayTags.Item_Unequip = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Item.Unequip"), FString("아이템 장착해제"));
	GameplayTags.Item_Drop = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Item.Drop"), FString("아이템 떨어뜨리기"));

}
