#pragma once
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ScalableFloat.h"
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
struct FDA_CharacterClass
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Character Class")
	TSubclassOf<UGameplayEffect> StatAttributes;

	UPROPERTY(EditDefaultsOnly, Category = "Character Class")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

	UPROPERTY(EditDefaultsOnly, Category = "Character Class")
	FScalableFloat ExpReward = FScalableFloat();
};

UCLASS()
class TDRPG_API UTDDA_CharacterClass : public UDataAsset
{
	GENERATED_BODY()

public:
	FDA_CharacterClass GetDA_ClassClass(ECharacterClass CharacterClass);

	UPROPERTY(EditDefaultsOnly, Category = "Character Class | Defaults")
	TMap<ECharacterClass, FDA_CharacterClass> CharacterClassTMap;

	UPROPERTY(EditDefaultsOnly, Category = "Character Class | Gameplay Effect")
	TSubclassOf<UGameplayEffect> StatAttributes_SetByCaller;
	UPROPERTY(EditDefaultsOnly, Category = "Character Class | Gameplay Effect")
	TSubclassOf<UGameplayEffect> SecondaryAttributes;
	UPROPERTY(EditDefaultsOnly, Category = "Character Class | Gameplay Effect")
	TSubclassOf<UGameplayEffect> VitalAttributes;

	UPROPERTY(EditDefaultsOnly, Category = "Character Class | Gameplay Ability")
	TArray<TSubclassOf<UGameplayAbility>> CommonAbilities;

	UPROPERTY(EditDefaultsOnly, Category = "Character Class | Damage")
	TObjectPtr<UCurveTable> DamageCalculationCoefficients;
};
