#pragma once
#include "CoreMinimal.h"
#include "GameplayTagContainer.h" // GameplayTag 타입 정보

/** TDGameplayTags
 *  native GamplayTags를 가지는 Singleton
 */
struct FTDGameplayTags
{
public:
	static const FTDGameplayTags& GetTDGameplayTags() { return GameplayTags; }
	static void InitializeNativeGameplayTags();

	FGameplayTag Attributes_Secondary_Armor;

private:
	static FTDGameplayTags GameplayTags;
};