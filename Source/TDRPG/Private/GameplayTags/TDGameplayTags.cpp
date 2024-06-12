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
	GameplayTags.Abilities_None = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.None"), FString("Abilities 없음"));

	GameplayTags.Abilities_Type_Active = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Type.Active"), FString("Abilities 타입: 액티브 스킬"));
	GameplayTags.Abilities_Type_Passive = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Type.Passive"), FString("Abilities 타입: 패시브 스킬"));
	GameplayTags.Abilities_Type_None = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Type.None"), FString("Abilities 타입: None"));

	GameplayTags.Abilities_Status_Locked = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Status.Locked"), FString("Abilities 상태: 잠금"));
	GameplayTags.Abilities_Status_Eligible = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Status.Eligible"), FString("Abilities 상태: 사용가능"));
	GameplayTags.Abilities_Status_Unlocked = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Status.Unlocked"), FString("Abilities 상태: 해금됨"));
	GameplayTags.Abilities_Status_Equipped = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Status.Equipped"), FString("Abilities 상태: 장착됨"));

	GameplayTags.Abilities_Skill_Fireball = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Skill.Fireball"), FString("Abilities: 파이어볼"));
	GameplayTags.Abilities_Skill_MagicArrow = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Skill.MagicArrow"), FString("Abilities: 매직 애로우"));
	GameplayTags.Abilities_Skill_ComboFlameShooting = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Skill.ComboFlameShooting"), FString("Abilities: 화염 난사"));
	GameplayTags.Abilities_Skill_Physical_Pummel = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Skill.Physical.Pummel"), FString("Abilities: 난화격"));
	GameplayTags.Abilities_Skill_Physical_FlyingKick = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Skill.Physical.FlyingKick"), FString("Abilities: 날라차기"));
	GameplayTags.Abilities_Skill_EnergyLance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Skill.EnergyLance"), FString("Abilities: 에너르기파"));
	GameplayTags.Abilities_Skill_Ice_FrozenField = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Skill.Ice.FrozenField"), FString("Abilities: 얼음 장판"));
	GameplayTags.Abilities_Skill_Fire_PillarOfFlame = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Skill.Fire.PillarOfFlame"), FString("Abilities: 불기둥"));
	GameplayTags.Abilities_Skill_StoneEdge = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Skill.StoneEdge"), FString("Abilities: 스톤엣지"));
	GameplayTags.Abilities_Skill_Meteor = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Skill.Meteor"), FString("Abilities: 메테오"));
	GameplayTags.Abilities_Skill_Flash = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Skill.Flash"), FString("Abilities: 대쉬"));
	GameplayTags.Abilities_Skill_Dash = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Skill.Dash"), FString("Abilities: 점멸"));

	// Cooldown
	GameplayTags.Cooldown_Skill_Fireball = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Skill.Fireball"), FString("Cooldown: 파이어볼"));
	GameplayTags.Cooldown_Skill_MagicArrow = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Skill.MagicArrow"), FString("Cooldown: 매직 애로우"));
	GameplayTags.Cooldown_Skill_ComboFlameShooting = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Skill.ComboFlameShooting"), FString("Cooldown: 화염 난사"));
	GameplayTags.Cooldown_Skill_Pummel = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Skill.Pummel"), FString("Cooldown: 난화격"));
	GameplayTags.Cooldown_Skill_FlyingKick = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Skill.FlyingKick"), FString("Cooldown: 날라차기"));
	GameplayTags.Cooldown_Skill_EnergyLance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Skill.EnergyLance"), FString("Cooldown: 에너르기파"));
	GameplayTags.Cooldown_Skill_FrozenField = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Skill.FrozenField"), FString("Cooldown: 얼음 장판"));
	GameplayTags.Cooldown_Skill_PillarOfFlame = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Skill.PillarOfFlame"), FString("Cooldown: 불기둥"));
	GameplayTags.Cooldown_Skill_StoneEdge = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Skill.StoneEdge"), FString("Cooldown: 스톤엣지"));
	GameplayTags.Cooldown_Skill_Meteor = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Skill.Meteor"), FString("Cooldown: 메테오"));
	GameplayTags.Cooldown_Skill_Flash = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Skill.Flash"), FString("Cooldown: 대쉬"));
	GameplayTags.Cooldown_Skill_Dash = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Skill.Dash"), FString("Cooldown: 점멸"));

	//** Meta
	// Damage Type
	GameplayTags.DamageType = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("DamageType"), FString("데미지 타입"));
	GameplayTags.DamageType_None = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("DamageType.None"), FString("데미지 타입 없음"));
	GameplayTags.DamageType_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("DamageType.Physical"), FString("물리공격 데미지 타입"));
	GameplayTags.DamageType_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("DamageType.Fire"), FString("불 공격 데미지 타입"));
	GameplayTags.DamageType_Ice = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("DamageType.Ice"), FString("얼음 데미지 타입"));
	GameplayTags.DamageType_Electronic = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("DamageType.Electronic"), FString("전기/에너지 데미지 타입"));
	GameplayTags.DamageType_Ultimate = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("DamageType.Ultimate"), FString("궁극기 데미지 타입"));

	// Resistance
	GameplayTags.Attributes_Resistance_None = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.None"), FString("데미지 저항 없음"));
	GameplayTags.Attributes_Resistance_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Physical"), FString("물리공격 데미지 저항"));
	GameplayTags.Attributes_Resistance_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Fire"), FString("불 데미지 저항"));
	GameplayTags.Attributes_Resistance_Ice = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Ice"), FString("얼음 데미지 저항"));
	GameplayTags.Attributes_Resistance_Electronic = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Electronic"), FString("전기/에너지 데미지 저항"));
	GameplayTags.Attributes_Resistance_Ultimate = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Ultimate"), FString("궁극기 데미지 저항"));

	// Debuffs
	GameplayTags.Debuff_None = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.None"), FString("디버프: 없음"));
	GameplayTags.Debuff_DotDamage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.DotDamage"), FString("디버프: 도트 데미지"));
	GameplayTags.Debuff_Slow = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Slow"), FString("디버프: 둔화"));
	GameplayTags.Debuff_Stun = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Stun"), FString("디버프: 기절"));

	GameplayTags.Debuff_Chance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Chance"), FString("디버프 걸릴 확률"));
	GameplayTags.Debuff_Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Damage"), FString("디버프 데미지 적용 수치"));
	GameplayTags.Debuff_Duration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Duration"), FString("디버프 지속시간"));
	GameplayTags.Debuff_Frequency = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Frequency"), FString("디버프 빈도"));

	// Meta Attributes
	GameplayTags.Attributes_Meta_IncomingExp = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Meta.IncomingExp"), FString("경험치 획득 Meta Attribute"));

	//**************************************************************************************
	// TMap 등록. DamageTypesToResistances
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.DamageType_Physical, GameplayTags.Attributes_Resistance_Physical);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.DamageType_Fire, GameplayTags.Attributes_Resistance_Fire);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.DamageType_Ice, GameplayTags.Attributes_Resistance_Ice);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.DamageType_Electronic, GameplayTags.Attributes_Resistance_Electronic);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.DamageType_Ultimate, GameplayTags.Attributes_Resistance_Ultimate);

	// TMap 등록. DamageTypes To Debuff
	GameplayTags.DamageTypesToDebuffs.Add(GameplayTags.DamageType_None, GameplayTags.Debuff_None);
	GameplayTags.DamageTypesToDebuffs.Add(GameplayTags.DamageType_Physical, GameplayTags.Debuff_None);
	GameplayTags.DamageTypesToDebuffs.Add(GameplayTags.DamageType_Fire, GameplayTags.Debuff_DotDamage);
	GameplayTags.DamageTypesToDebuffs.Add(GameplayTags.DamageType_Ice, GameplayTags.Debuff_Slow);
	GameplayTags.DamageTypesToDebuffs.Add(GameplayTags.DamageType_Electronic, GameplayTags.Debuff_Stun);
	GameplayTags.DamageTypesToDebuffs.Add(GameplayTags.DamageType_Ultimate, GameplayTags.Debuff_Stun);
	//**************************************************************************************

	// Effect
	GameplayTags.Effect_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Effect.HitReact"), FString("피격 시 GameplayTag Granted 반응"));

	// Event
	GameplayTags.Event_Montage_Fireball = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Event.Montage.Fireball"), FString("파이어볼"));
	GameplayTags.Event_Montage_MagicArrow = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Event.Montage.MagicArrow"), FString("매직 애로우"));
	GameplayTags.Event_Montage_ComboFlameShooting = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Event.Montage.ComboFlameShooting"), FString("화염 난사"));
	GameplayTags.Event_Montage_Pummel = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Event.Montage.Pummel"), FString("난화격"));
	GameplayTags.Event_Montage_FlyingKick = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Event.Montage.FlyingKick"), FString("날라차기"));
	GameplayTags.Event_Montage_EnergyLance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Event.Montage.EnergyLance"), FString("에너르기파"));
	GameplayTags.Event_Montage_FrozenField = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Event.Montage.FrozenField"), FString("얼음 장판"));
	GameplayTags.Event_Montage_PillarOfFlame = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Event.Montage.PillarOfFlame"), FString("불기둥"));
	GameplayTags.Event_Montage_StoneEdge = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Event.Montage.StoneEdge"), FString("스톤엣지"));
	GameplayTags.Event_Montage_Meteor = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Event.Montage.Meteor"), FString("메테오"));
	GameplayTags.Event_Montage_Flash = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Event.Montage.Flash"), FString("점멸"));
	GameplayTags.Event_Montage_Dash = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Event.Montage.Dash"), FString("대쉬"));

	// Socket
	GameplayTags.Socket_Weapon = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Socket.Weapon"),FString("무기 소켓"));
	GameplayTags.Socket_Head = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Socket.Head"), FString("머리(대가리) 소켓"));
	GameplayTags.Socket_RightHand = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Socket.RightHand"),FString("오른손 소켓"));
	GameplayTags.Socket_LeftHand = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Socket.LeftHand"),FString("왼손 소켓"));
	GameplayTags.Socket_RightLeg = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Socket.RightLeg"), FString("오른발 소켓"));
	GameplayTags.Socket_LeftLeg = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Socket.LeftLeg"), FString("왼발 소켓"));
	GameplayTags.Socket_Trail = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Socket.Trail"),FString("공격 시 무기/손 트레일이펙트 위치"));

	// Montage
	GameplayTags.Montage_ATK_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.ATK.1"), FString("공격 1"));
	GameplayTags.Montage_ATK_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.ATK.2"), FString("공격 2"));
	GameplayTags.Montage_ATK_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.ATK.3"), FString("공격 3"));
	GameplayTags.Montage_ATK_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.ATK.4"), FString("공격 4"));

	//************************************************************************************************
	// Enhanced Input
	GameplayTags.InputTag_Q = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.Q"), FString("Q버튼 Input Tag"));
	GameplayTags.InputTag_W = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.W"), FString("W버튼 Input Tag"));
	GameplayTags.InputTag_E = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.E"),FString("E버튼 Input Tag"));
	GameplayTags.InputTag_R = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.R"),FString("R버튼 Input Tag"));
	GameplayTags.InputTag_A = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.A"),FString("A버튼 Input Tag"));
	GameplayTags.InputTag_S = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.S"),FString("S버튼 Input Tag"));
	GameplayTags.InputTag_D = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.D"),FString("D버튼 Input Tag"));
	GameplayTags.InputTag_F = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.F"),FString("F버튼 Input Tag"));
	GameplayTags.InputTag_V = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.V"),FString("V버튼 Input Tag"));
	GameplayTags.InputTag_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.LMB"), FString("왼쪽 마우스 버튼 Input Tag"));
	GameplayTags.InputTag_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.RMB"), FString("오른쪽 마우스 버튼 Input Tag"));
	GameplayTags.InputTag_Spacebar = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.Spacebar"), FString("스페이스바 버튼 Input Tag"));
	GameplayTags.InputTag_Passive_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.Passive.1"),FString("패시브1 Input Tag"));
	GameplayTags.InputTag_Passive_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.Passive.2"),FString("패시브2 Input Tag"));

	// Input 상태: Pressed, Held, Released, CursorTrace
	GameplayTags.BlockTag_InputPressed = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("BlockTag.InputPressed"), FString("Block: Input Pressed 상태"));
	GameplayTags.BlockTag_InputHeld = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("BlockTag.InputHeld"), FString("Block: Input Held 상태"));
	GameplayTags.BlockTag_InputReleased = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("BlockTag.InputReleased"), FString("Block: Input Released 상태"));
	GameplayTags.BlockTag_CursorTrace = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("BlockTag.CursorTrace"), FString("Block: Input Trace 상태"));

	//************************************************************************************************

	// Gameplay Cues
	GameplayTags.GameplayCue_MeleeImpact = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.MeleeImpact"), FString("MeleeAttack 피격 시 이펙트"));
	GameplayTags.GameplayCue_Burst = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.Burst"), FString("Burst 사운드"));
	GameplayTags.GameplayCue_Loop = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.Loop"), FString("Loop 사운드"));
	GameplayTags.GameplayCue_FrozenField = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.FrozenField"), FString("FrozenField 스킬 이펙트"));
	GameplayTags.GameplayCue_PillarOfFlame = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.PillarOfFlame"), FString("불기둥 스킬 이펙트"));
	GameplayTags.GameplayCue_ComboFlameShooting = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.ComboFlameShooting"), FString("불 날리기 스킬 이펙트"));

	// Inventory + Item
	GameplayTags.Item_Equip = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Item.Equip"), FString("아이템 장착"));
	GameplayTags.Item_EquipNext = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Item.EquipNext"), FString("다음 아이템 장착"));
	GameplayTags.Item_Unequip = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Item.Unequip"), FString("아이템 장착해제"));
	GameplayTags.Item_Drop = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Item.Drop"), FString("아이템 떨어뜨리기"));

}
