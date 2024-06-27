#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TDGameModeBase.generated.h"

class UTDGameInstance;
class UTDDA_CharacterClass;
class UTDDA_Ability;
class UTDDA_DropItem;
class UTDDA_ItemGE;
class USaveGame;
class UTDMVVM_Slot;
class UTDSaveGame_Load;

/** ���� ���� Ŭ���� ���.
 *  GAS - CharacterClass, Ability ���.
 */

UCLASS()
class TDRPG_API ATDGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	//********************************************************
	//** GAS
	UPROPERTY(EditDefaultsOnly, Category = "Character Class")
	TObjectPtr<UTDDA_CharacterClass> TDDACharacterClass;

	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	TObjectPtr<UTDDA_Ability> TDDAAbility;
	//********************************************************

	//********************************************************
	//** Monster Drop Items & Player loot tiers items
	UPROPERTY(EditDefaultsOnly, Category = "Loot Tiers")
	TObjectPtr<UTDDA_DropItem> TDDADropItem;
	//********************************************************

};
