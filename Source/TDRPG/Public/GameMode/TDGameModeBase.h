#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TDGameModeBase.generated.h"

class UTDGameInstance;
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
	FORCEINLINE TObjectPtr<UTDGameInstance> GetTDGameInstance();

	void SaveSlotData(UTDMVVM_Slot* LoadSlot, int32 SlotIndex); // 게임 저장
	UTDSaveGame_Load* GetSaveSlotData(const FString& SlotName, int32 SlotIdx) const;
	static void DeleteSlot(const FString& SlotName, int32 SlotIndex);

	UTDSaveGame_Load* RetrieveInGameSaveData();
	void SaveInGameProgressData(UTDSaveGame_Load* SaveObject);

	void SaveWorldState(UWorld* World); // SaveMap에 월드 내 Actor 데이터들을 저장하는 함수.
	void LoadWorldState(UWorld* World); // SaveMap에 저장된 Actor 데이터를 로드하는 함수.

	void TravelToMap(UTDMVVM_Slot* Slot);
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	//********************************************************
	//** SaveGame, Map
	UPROPERTY(EditDefaultsOnly, Category = "Save Game")
	TSubclassOf<USaveGame> LoadScreenSaveGameClass;

	UPROPERTY(EditDefaultsOnly, Category = "Save Game")
	FName DefaultPlayerStartTag;

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

private:
	UPROPERTY()
	TObjectPtr<UTDGameInstance> TDGameInstance;
};
