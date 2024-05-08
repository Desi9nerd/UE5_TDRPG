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
	UFUNCTION(BlueprintPure, meta = (DefaultToSelf = "WorldContextObject"))
	static bool MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& ResultWCParams, ATDHUD*& OutTDHUD);

	UFUNCTION(BlueprintPure, meta = (DefaultToSelf = "WorldContextObject"))
	static UTDWidgetControllerOverlay* GetTDWidgetControllerOverlay(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, meta = (DefaultToSelf = "WorldContextObject"))
	static UTDWidgetControllerAttributeMenu* GetTDWidgetControllerAttributeMenu(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, meta = (DefaultToSelf = "WorldContextObject"))
	static UTDWidgetControllerSkillMenu* GetTDWidgetControllerSKillMenu(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable)
	static void InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC);

	UFUNCTION(BlueprintCallable)
	static void GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass); // GameplayAbility 초기화

	UFUNCTION(BlueprintCallable)
	static UTDDA_CharacterClass* GetTDDA_CharacterClass(const UObject* WorldContextObject); // TDGameMode의 UTDDACharacterClass를 가져오는 함수

	UFUNCTION(BlueprintCallable)
	static UTDDA_Ability* GetTDDA_Ability(const UObject* WorldContextObject);

	//********************************************************
	//*  TDAbilityType.h.cpp와 연동.
	//** Block Hit, Critical Hit, Debuff
	UFUNCTION(BlueprintCallable)
	static FGameplayTag GetDamageType(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintCallable)
	static float GetDebuffDamage(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintCallable)
	static float GetDebuffDuration(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintCallable)
	static float GetDebuffFrequency(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintCallable)
	static void SetIsBlockedHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockedHit);
	UFUNCTION(BlueprintCallable)
	static void SetIsCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsCriticalHit);
	UFUNCTION(BlueprintCallable)
	static void SetDebuff(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInDebuff);
	UFUNCTION(BlueprintCallable)
	static void SetDebuffDamage(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InDebuffDamage);
	UFUNCTION(BlueprintCallable)
	static void SetDebuffDuration(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InDebuffDuration);
	UFUNCTION(BlueprintCallable)
	static void SetDebuffFrequency(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InDebuffFrequency);
	static void SetDamageType(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const FGameplayTag& InDamageType);

	UFUNCTION(BlueprintPure)
	static bool IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintPure)
	static bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintPure)
	static bool IsDebuff(const FGameplayEffectContextHandle& EffectContextHandle);
	//********************************************************

	UFUNCTION(BlueprintCallable)
	static void GetLivePlayersWithinRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin);

	UFUNCTION(BlueprintPure)
	static bool IsSameTeam(AActor* FirstActor, AActor* SecondActor);


	static int32 GetExpRewardForClassAndLevel(const UObject* WorldContextObject, ECharacterClass CharacterClass, int32 CharacterLevel);

	UFUNCTION(BlueprintCallable)
	static FGameplayEffectContextHandle ApplyDamageEffect(const FDamageEffectParams& DamageEffectParams);

	//********************************************************
	//** 테스트용. 아래의 BP 버젼은 추후에 삭제할 예정.
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	static bool GetItemInfo(const FString& InItemName, FItem& OutItemInfo);
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	static bool GetInventoryCategoryInfo(const FString& InItemCategoryName, FInventoryCategory& OutCategoryInfo);
	//********************************************************
};
