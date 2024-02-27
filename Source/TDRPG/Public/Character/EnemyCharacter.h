#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"
#include "Interface/IEnemy.h"
#include "EnemyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class TDRPG_API AEnemyCharacter : public ABaseCharacter, public IIEnemy
{
	GENERATED_BODY()

public:
	AEnemyCharacter();
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
};
