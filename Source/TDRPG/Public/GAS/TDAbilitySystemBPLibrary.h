#pragma once
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Data/TDDA_CharacterClass.h"
#include "Library/TDItemLibrary.h"
#include "TDAbilitySystemBPLibrary.generated.h"

struct FDamageEffectParams;
struct FGameplayEffectContextHandle;
class UAbilitySystemComponent;
class UTDWidgetControllerOverlay;
class UTDWidgetControllerAttributeMenu;
class UTDWidgetControllerSkillMenu;
class UTDItemStaticData;
struct FWidgetControllerParams;
class ATDHUD;
class UTDDA_Ability;

/**
 * 
 */

UCLASS()
class TDRPG_API UTDAbilitySystemBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "TDAbilitySystemBPLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static bool MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& ResultWCParams, ATDHUD*& OutTDHUD);

	UFUNCTION(BlueprintPure, Category = "TDAbilitySystemBPLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static UTDWidgetControllerOverlay* GetTDWidgetControllerOverlay(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "TDAbilitySystemBPLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static UTDWidgetControllerAttributeMenu* GetTDWidgetControllerAttributeMenu(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "TDAbilitySystemBPLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static UTDWidgetControllerSkillMenu* GetTDWidgetControllerSKillMenu(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "TDAbilitySystemBPLibrary|CharacterClass")
	static void InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC);

	UFUNCTION(BlueprintCallable, Category = "TDAbilitySystemBPLibrary|CharacterClass")
	static void GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass); // GameplayAbility 초기화

	UFUNCTION(BlueprintCallable, Category = "TDAbilitySystemBPLibrary|CharacterClass")
	static UTDDA_CharacterClass* GetTDDA_CharacterClass(const UObject* WorldContextObject); // TDGameMode의 UTDDACharacterClass를 가져오는 함수

	UFUNCTION(BlueprintCallable, Category = "TDAbilitySystemBPLibrary|Ability")
	static UTDDA_Ability* GetTDDA_Ability(const UObject* WorldContextObject);

	//********************************************************
	//** TDAbilityType.h.cpp와 연동.
	//** Block Hit, Critical Hit, Debuff
	UFUNCTION(BlueprintCallable, Category = "TDAbilitySystemBPLibrary|GameplayEffects")
	static void SetIsBlockedHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockedHit);
	UFUNCTION(BlueprintCallable, Category = "TDAbilitySystemBPLibrary|GameplayEffects")
	static void SetIsCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsCriticalHit);
	UFUNCTION(BlueprintCallable, Category = "TDAbilitySystemBPLibrary|GameplayEffects")
	static void SetDebuff(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInDebuff);
	UFUNCTION(BlueprintCallable, Category = "TDAbilitySystemBPLibrary|GameplayEffects")
	static void SetDebuffDamage(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InDebuffDamage);
	UFUNCTION(BlueprintCallable, Category = "TDAbilitySystemBPLibrary|GameplayEffects")
	static void SetDebuffDuration(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InDebuffDuration);
	UFUNCTION(BlueprintCallable, Category = "TDAbilitySystemBPLibrary|GameplayEffects")
	static void SetDebuffFrequency(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InDebuffFrequency);
	static void SetDamageType(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const FGameplayTag& InDamageType);

	UFUNCTION(BlueprintPure, Category = "TDAbilitySystemBPLibrary|GameplayEffects")
	static bool IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintPure, Category = "TDAbilitySystemBPLibrary|GameplayEffects")
	static bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintPure, Category = "TDAbilitySystemBPLibrary|GameplayEffects")
	static bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);
	//********************************************************

	UFUNCTION(BlueprintCallable, Category = "TDAbilitySystemBPLibrary|GameplayMechanics")
	static void GetLivePlayersWithinRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin);

	UFUNCTION(BlueprintPure, Category = "TDAbilitySystemBPLibrary|GameplayMechanics")
	static bool IsSameTeam(AActor* FirstActor, AActor* SecondActor);


	static int32 GetExpRewardForClassAndLevel(const UObject* WorldContextObject, ECharacterClass CharacterClass, int32 CharacterLevel);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|DamageEffect")
	static FGameplayEffectContextHandle ApplyDamageEffect(const FDamageEffectParams& DamageEffectParams);

	//********************************************************
	//** 테스트용. 아래의 BP 버젼은 추후에 삭제할 예정.
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	static bool GetItemInfo(const FString& InItemName, FItem& OutItemInfo);
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	static bool GetInventoryCategoryInfo(const FString& InItemCategoryName, FInventoryCategory& OutCategoryInfo);
	//********************************************************
};
