#include "GameplayTags/TDGameplayTags.h"
#include "GameplayTagsManager.h"

FTDGameplayTags FTDGameplayTags::GameplayTags; // static GamplayTags instance

void FTDGameplayTags::InitializeNativeGameplayTags()
{
	GameplayTags.Attributes_Secondary_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.Armor"), FString("Reduces damage taken, improves Block Chance"));
}
