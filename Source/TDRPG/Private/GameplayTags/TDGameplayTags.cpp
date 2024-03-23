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

	// Event
	GameplayTags.Event_Montage_Fireball = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Event.Montage.Fireball"), FString("파이어볼"));

	// Enhanced Input
	GameplayTags.InputTag_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.LMB"), FString("왼쪽 마우스 버튼 Input Tag"));
	GameplayTags.InputTag_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.RMB"), FString("오른쪽 마우스 버튼 Input Tag"));
	GameplayTags.InputTag_Q = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.Q"), FString("Q버튼 Input Tag"));
	GameplayTags.InputTag_W = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.W"), FString("W버튼 Input Tag"));
	GameplayTags.InputTag_E = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.E"),FString("E버튼 Input Tag"));
	GameplayTags.InputTag_R = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.R"),FString("R버튼 Input Tag"));
}
