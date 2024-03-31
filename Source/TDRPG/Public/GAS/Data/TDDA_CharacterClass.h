#pragma once
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TDDA_CharacterClass.generated.h"

class UGameplayEffect;
class UGameplayAbility;

/** 캐릭터의 데이터를 집어넣는 클래스
 *  스탯, GameplayAbilities, 데미지처리CT
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

	UPROPERTY(EditDefaultsOnly, Category = "Class Defaults")
	TSubclassOf<UGameplayEffect> StatAttributes;

	UPROPERTY(EditDefaultsOnly, Category = "Class Defaults")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;
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

	UPROPERTY(EditDefaultsOnly, Category = "Character Class | Damage")
	TObjectPtr<UCurveTable> DamageCalculationCoefficients;
};
