#include "Spawn/TDTargetPoint_Spawn.h"
#include "Character/TDEnemyCharacter.h"

void ATDTargetPoint_Spawn::SpawnEnemy()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	// 적 몬스터 스폰 시키기
	ATDEnemyCharacter* Enemy = GetWorld()->SpawnActorDeferred<ATDEnemyCharacter>(EnemyCharacterClass, GetActorTransform());
	Enemy->SetEnemyLevel(EnemyLevel);
	Enemy->SetCharacterClass(CharacterClass);
	Enemy->FinishSpawning(GetActorTransform());
	Enemy->SpawnDefaultController();
}
