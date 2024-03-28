#pragma once
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Data/TDDA_CharacterClass.h"
#include "TDAbilitySystemBPLibrary.generated.h"

class UAbilitySystemComponent;
class UTDWidgetControllerOverlay;
class UTDWidgetControllerAttributeMenu;

/**
 * 
 */

UCLASS()
class TDRPG_API UTDAbilitySystemBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "TDAbilitySystemBPLibrary|WidgetController")
	static UTDWidgetControllerOverlay* GetWidgetControllerOverlay(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "TDAbilitySystemBPLibrary|WidgetController")
	static UTDWidgetControllerAttributeMenu* GetAttributeMenuWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "TDAbilitySystemBPLibrary|CharacterClassDefaults")
	static void InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC);

	UFUNCTION(BlueprintCallable, Category = "TDAbilitySystemBPLibrary|CharacterClassDefaults")
	static void GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC); // GameplayAbility �ʱ�ȭ

	UFUNCTION(BlueprintCallable, Category = "TDAbilitySystemBPLibrary|CharacterClassDefaults")
	static UTDDA_CharacterClass* GetTDDA_CharacterClass(const UObject* WorldContextObject); // TDGameMode�� UTDDACharacterClass�� �������� �Լ�
};
