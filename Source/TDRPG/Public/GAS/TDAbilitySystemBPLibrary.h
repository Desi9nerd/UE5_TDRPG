﻿#pragma once
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "AbilitySystemComponent.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Data/TDDA_CharacterClass.h"
#include "Library/TDItemLibrary.h"
#include "TDAbilitySystemBPLibrary.generated.h"

struct FDamageEffectParams;
struct FGameplayEffectContextHandle;
class UTDSaveGame_Load;
class UAbilitySystemComponent;
class UTDWidgetControllerOverlay;
class UTDWidgetControllerAttributeMenu;
class UTDWidgetControllerSkillMenu;
class UTDItemStaticData;
struct FWidgetControllerParams;
class ATDHUD;
class UTDDA_Ability;
class UTDDA_DropItem;

/**
 * 
 */

UCLASS()
class TDRPG_API UTDAbilitySystemBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	//********************************************************
	//* SaveGame
	UFUNCTION(BlueprintCallable)
	static void InitializeDefaultAttributesFromSaveData(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, UTDSaveGame_Load* SaveGame);
	//********************************************************

	//********************************************************
	//* Widget Controller
	UFUNCTION(BlueprintPure, meta = (DefaultToSelf = "WorldContextObject"))
	static bool MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& ResultWCParams, ATDHUD*& OutTDHUD);

	UFUNCTION(BlueprintPure, meta = (DefaultToSelf = "WorldContextObject"))
	static UTDWidgetControllerOverlay* GetTDWidgetControllerOverlay(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, meta = (DefaultToSelf = "WorldContextObject"))
	static UTDWidgetControllerAttributeMenu* GetTDWidgetControllerAttributeMenu(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, meta = (DefaultToSelf = "WorldContextObject"))
	static UTDWidgetControllerSkillMenu* GetTDWidgetControllerSKillMenu(const UObject* WorldContextObject);
	//********************************************************

	//********************************************************
	//* Gameplay Ability System
	UFUNCTION(BlueprintCallable)
	static void InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC);

	UFUNCTION(BlueprintCallable)
	static void GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass); // GameplayAbility 초기화

	UFUNCTION(BlueprintCallable)
	static UTDDA_CharacterClass* GetTDDA_CharacterClass(const UObject* WorldContextObject); // TDGameMode의 UTDDACharacterClass를 가져오는 함수

	UFUNCTION(BlueprintCallable)
	static UTDDA_Ability* GetTDDA_Ability(const UObject* WorldContextObject);
	//********************************************************

	//********************************************************
	//*  Effect Context
	//** TDAbilityType.h.cpp와 연동. Block Hit, Critical Hit, Debuff, Ragdoll, Knockback
	UFUNCTION(BlueprintCallable)
	static FGameplayTag GetDamageType(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintCallable)
	static float GetDebuffDamage(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintCallable)
	static float GetDebuffDuration(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintCallable)
	static float GetDebuffFrequency(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintPure)
	static FVector GetRagdollImpulse(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintPure)
	static FVector GetKnockbackImpulse(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintPure)
	static float GetRadialDamageInnerRadius(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure)
	static float GetRadialDamageOuterRadius(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure)
	static FVector GetRadialDamageOrigin(const FGameplayEffectContextHandle& EffectContextHandle);

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
	UFUNCTION(BlueprintCallable)
	static void SetDamageType(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const FGameplayTag& InDamageType);
	UFUNCTION(BlueprintCallable)
	static void SetRagdollImpulse(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const FVector& InRagdollImpulse);
	UFUNCTION(BlueprintCallable)
	static void SetKnockbackImpulse(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const FVector& InKnockbackImpulse);
	UFUNCTION(BlueprintCallable)
	static void SetIsRadialDamage(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool InbRadialDamage);

	UFUNCTION(BlueprintCallable)
	static void SetRadialDamageInnerRadius(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InInnerRadius);

	UFUNCTION(BlueprintCallable)
	static void SetRadialDamageOuterRadius(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InOuterRadius);

	UFUNCTION(BlueprintCallable)
	static void SetRadialDamageOrigin(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const FVector& InOrigin);


	UFUNCTION(BlueprintPure)
	static bool IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintPure)
	static bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintPure)
	static bool IsDebuff(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintPure)
	static bool IsRadialDamage(const FGameplayEffectContextHandle& EffectContextHandle);
	//********************************************************

	//********************************************************
	//*  Damage Effect Params
	UFUNCTION(BlueprintCallable)
	static void SetIsRadialDamageEffectParam(UPARAM(ref) FDamageEffectParams& DamageEffectParams, bool bIsRadial, float InnerRadius, float OuterRadius, FVector Origin);

	UFUNCTION(BlueprintCallable)
	static void SetKnockbackDirection(UPARAM(ref) FDamageEffectParams& DamageEffectParams, FVector KnockbackDirection, float Magnitude = 0.f);

	UFUNCTION(BlueprintCallable)
	static void SetRagdollImpulseDirection(UPARAM(ref) FDamageEffectParams& DamageEffectParams, FVector ImpulseDirection, float Magnitude = 0.f);

	UFUNCTION(BlueprintCallable)
	static void SetTargetEffectParamsASC(UPARAM(ref) FDamageEffectParams& DamageEffectParams, UAbilitySystemComponent* InASC);
	//********************************************************


	UFUNCTION(BlueprintPure)
	static bool IsSameTeam(AActor* FirstActor, AActor* SecondActor);


	static int32 GetExpRewardForClassAndLevel(const UObject* WorldContextObject, ECharacterClass CharacterClass, int32 CharacterLevel);

	UFUNCTION(BlueprintCallable)
	static FGameplayEffectContextHandle ApplyDamageEffect(const FDamageEffectParams& DamageEffectParams);

	//********************************************************
	//** 스킬 공격 관련
	UFUNCTION(BlueprintCallable)
	static void GetLivePlayersWithinRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin); // 플레이어 기준에서 특정 반경 내의 살아있는 모든 적들 OutOverlappingActors에 업데이트

	UFUNCTION(BlueprintCallable)
	static void GetClosestTargets(int32 MaxTargets, const TArray<AActor*>& InActors, TArray<AActor*>& OutClosestTargets, const FVector& InOrigin);
	//********************************************************

	//********************************************************
	//** Monster Drop Item Data.
	UFUNCTION(BlueprintCallable, meta = (DefaultToSelf = "WorldContextObject"))
	static UTDDA_DropItem* GetTDDA_DropItem(const UObject* WorldContextObject);
	//********************************************************

	//********************************************************
	//** 테스트용. 아래의 BP 버젼은 추후에 삭제할 예정.
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	static bool GetItemInfo(const FString& InItemName, FItem& OutItemInfo);
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	static bool GetInventoryCategoryInfo(const FString& InItemCategoryName, FInventoryCategory& OutCategoryInfo);
	//********************************************************
};
