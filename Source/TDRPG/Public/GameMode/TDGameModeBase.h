#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TDGameModeBase.generated.h"

class UTDDA_CharacterClass;
class UTDDA_Ability;
class USaveGame;
class UTDMVVM_Slot;
class UTDSaveGame_Load;

/** 게임 저장 클래스 등록.
 *  GAS - CharacterClass, Ability 등록.
 */

UCLASS()
class TDRPG_API ATDGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	void SaveSlotData(UTDMVVM_Slot* LoadSlot, int32 SlotIndex); // 게임 저장
	UTDSaveGame_Load* GetSaveSlotData(const FString& SlotName, int32 SlotIdx) const;
	static void DeleteSlot(const FString& SlotName, int32 SlotIndex);
	void TravelToMap(UTDMVVM_Slot* Slot);

	//********************************************************
	//** SaveGame, Map
	UPROPERTY(EditDefaultsOnly, Category = "Save Game")
	TSubclassOf<USaveGame> LoadScreenSaveGameClass;

	UPROPERTY(EditDefaultsOnly, Category = "Save Game")
	FString DefaultMapName;

	UPROPERTY(EditDefaultsOnly, Category = "Save Game")
	TSoftObjectPtr<UWorld> DefaultMap; // 기본맵.

	UPROPERTY(EditDefaultsOnly, Category = "Save Game")
	TMap<FString, TSoftObjectPtr<UWorld>> Maps;
	//********************************************************

	//********************************************************
	//** GAS
	UPROPERTY(EditDefaultsOnly, Category = "Character Class")
	TObjectPtr<UTDDA_CharacterClass> TDDACharacterClass;

	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	TObjectPtr<UTDDA_Ability> TDDAAbility;
	//********************************************************

protected:
	virtual void BeginPlay() override;
};
