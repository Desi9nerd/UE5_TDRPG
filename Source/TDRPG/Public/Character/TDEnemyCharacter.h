#pragma once

#include "CoreMinimal.h"
#include "Character/TDBaseCharacter.h"
#include "Interface/IEnemy.h"
#include "TDEnemyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class TDRPG_API ATDEnemyCharacter : public ATDBaseCharacter, public IIEnemy
{
	GENERATED_BODY()

public:
	ATDEnemyCharacter();

	//** IEnemy 함수
	virtual void HighlightActor() override;	  // 외곽선 효과 On
	virtual void UnHighlightActor() override; // 외곽선 효과 Off

protected:
	virtual void BeginPlay() override;
};
