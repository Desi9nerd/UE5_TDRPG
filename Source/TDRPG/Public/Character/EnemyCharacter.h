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

	//** IEnemy 함수
	virtual void HighlightActor() override;	  // 외곽선 효과 On
	virtual void UnHighlightActor() override; // 외곽선 효과 Off

protected:
	virtual void BeginPlay() override;
};
