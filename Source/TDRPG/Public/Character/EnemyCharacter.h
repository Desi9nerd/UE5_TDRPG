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

	//** IEnemy �Լ�
	virtual void HighlightActor() override;	  // �ܰ��� ȿ�� On
	virtual void UnHighlightActor() override; // �ܰ��� ȿ�� Off

protected:
	virtual void BeginPlay() override;
};
