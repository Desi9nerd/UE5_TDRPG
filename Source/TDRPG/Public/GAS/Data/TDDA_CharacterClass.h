#pragma once
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TDDA_CharacterClass.generated.h"

class UGameplayEffect;
class UGameplayAbility;

/**
 * 
 */

UENUM(BlueprintType)
enum class ECharacterClass : uint8
{
	Warrior,
	Lich,
	Archer
};

USTRUCT(BlueprintType)
struct FCharacterClassDefaultInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Character Class | Gameplay Effect")
	TSubclassOf<UGameplayEffect>StatAttributes;
};

UCLASS()
class TDRPG_API UTDDA_CharacterClass : public UDataAsset
{
	GENERATED_BODY()

public:
	FCharacterClassDefaultInfo GetClassDefaultInfo(ECharacterClass CharacterClass);

	UPROPERTY(EditDefaultsOnly, Category = "Character Class | Defaults")
	TMap<ECharacterClass, FCharacterClassDefaultInfo> CharacterClassInformation;

	UPROPERTY(EditDefaultsOnly, Category = "Character Class | Gameplay Effect")
	TSubclassOf<UGameplayEffect> SecondaryAttributes;
	UPROPERTY(EditDefaultsOnly, Category = "Character Class | Gameplay Effect")
	TSubclassOf<UGameplayEffect> VitalAttributes;

	UPROPERTY(EditDefaultsOnly, Category = "Character Class | Gameplay Ability")
	TArray<TSubclassOf<UGameplayAbility>> CommonAbilities;
};
