#pragma once
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/SaveGame.h"
#include "TDSaveGame_Load.generated.h"

class UGameplayAbility;

/** 슬롯의 이름, 인덱스, 플레이어 이름 설정
 *  게임 데이터를 저장함.
 */

/* Q. 데이터를 포인터 배열이 아니라 구조체 배열 형태로 저장하는 이유는? (ex. TArray<FSavedMap> SavedMaps)
 * A. 게임을 껏다가 다시 켰을때 같은 메모리 주소를 사용한다는 보장이 없기 때문읻. 포인터로 주소지를 가르키는 대신 구조체에 담아야 한다. 즉, 메모리 주소가 아닌 데이터를 저장한다.
 */

UENUM(BlueprintType)
enum ESG_SaveSlotStatus
{
	Vacant = 0,
	EnterName = 1,
	Taken = 2
};

//---------------------------------------------------------
USTRUCT()
struct FSavedActor // 저장될 Actor 데이터
{
	GENERATED_BODY()

	UPROPERTY()
	FName ActorName = FName();

	UPROPERTY()
	FTransform Transform = FTransform();

	UPROPERTY()
	TArray<uint8> Bytes; // Actor에서 Serialized된 변수들. SaveGame 관련된 것들이 마킹된다.
};

inline bool operator==(const FSavedActor& Left, const FSavedActor& Right)
{
	return Left.ActorName == Right.ActorName;
}

USTRUCT()
struct FSavedMap // 저장될 Map 데이터 (저장될 Actor 데이터를 포함)
{
	GENERATED_BODY()

	UPROPERTY()
	FString MapAssetName = FString();

	UPROPERTY()
	TArray<FSavedActor> SavedActors;
};
//---------------------------------------------------------

//---------------------------------------------------------
USTRUCT(BlueprintType)
struct FSavedAbility
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ClassDefaults")
	TSubclassOf<UGameplayAbility> GameplayAbility;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag AbilityTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag AbilityStatus = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag AbilitySlot = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag AbilityType = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 AbilityLevel = 1;
};

inline bool operator==(const FSavedAbility& Left, const FSavedAbility& Right)
{
	return Left.AbilityTag.MatchesTagExact(Right.AbilityTag);
}
//---------------------------------------------------------

UCLASS()
class TDRPG_API UTDSaveGame_Load : public USaveGame
{
	GENERATED_BODY()
	
public:
	FSavedMap GetSavedMapWithMapName(const FString& InMapName);
	bool HasMap(const FString& InMapName);

	//********************************************************
	//** SaveGame 데이터.
	UPROPERTY()
	FString SlotName = FString();

	UPROPERTY()
	int32 SlotIndex = 0;

	UPROPERTY()
	FString PlayerName = FString("Default Player Name");
	UPROPERTY()
	FString MapName = FString("Default Map Name");

	UPROPERTY()
	FString MapAssetName = FString("Default Map Asset Name");

	UPROPERTY()
	FName PlayerStartTag = FName();

	UPROPERTY()
	TEnumAsByte<ESG_SaveSlotStatus> SaveSlotStatus = Vacant;

	UPROPERTY()
	bool bFirstTimeLoadIn = true; // SaveData 생성 후 처음 게임을 시작하는지/아닌지.

	UPROPERTY()
	TArray<FSavedMap> SavedMaps; // 레벨맵 정보를 담는 배열.
	//********************************************************

	//********************************************************
	//** Player 정보.
	UPROPERTY()
	int32 PlayerLevel = 1;
	UPROPERTY()
	int32 Exp = 0;
	UPROPERTY()
	int32 SkillPoints = 0;
	UPROPERTY()
	int32 AttributePoints = 0;
	UPROPERTY()
	float Strength = 0;
	UPROPERTY()
	float Intelligence = 0;
	UPROPERTY()
	float Resilience = 0;
	UPROPERTY()
	float Vigor = 0;
	//********************************************************

	//********************************************************
	//** GAS Abilities
	UPROPERTY()
	TArray<FSavedAbility> SavedAbilities;
	//********************************************************
};
