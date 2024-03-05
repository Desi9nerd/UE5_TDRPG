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

	//** IEnemy �Լ�
	virtual void HighlightActor() override;	  // �ܰ��� ȿ�� On
	virtual void UnHighlightActor() override; // �ܰ��� ȿ�� Off

protected:
	virtual void BeginPlay() override;
};
