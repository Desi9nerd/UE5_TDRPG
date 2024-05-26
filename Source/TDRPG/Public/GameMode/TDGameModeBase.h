#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TDGameModeBase.generated.h"

class ATDItemActor;
class UTDDA_CharacterClass;
class UTDDA_Ability;

/**
 * 
 */
UCLASS()
class TDRPG_API ATDGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	void RequestDropItem(APlayerController* InPlayerController, TSubclassOf<ATDItemActor> ItemActor, int32 ItemQuantity, FVector SpawnLocation);

	UPROPERTY(EditDefaultsOnly, Category = "Character Class")
	TObjectPtr<UTDDA_CharacterClass> TDDACharacterClass;

	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	TObjectPtr<UTDDA_Ability> TDDAAbility;
};
