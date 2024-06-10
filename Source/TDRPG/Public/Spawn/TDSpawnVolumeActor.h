#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/ISaveGame.h"
#include "TDSpawnVolumeActor.generated.h"

class UBoxComponent;
class ATDTargetPoint_Spawn;

/** 적 몬스터를 스폰되는 트리거가 되는 충돌박스.
 *  충돌 시 지정해둔 TargetPoint에 몬스터가 스폰되도록 설정함.
 *  ATDTargetPoint_Spawn과 함께 사용하여 몬스터 스폰시킴.
 */
UCLASS()
class TDRPG_API ATDSpawnVolumeActor : public AActor, public IISaveGame
{
	GENERATED_BODY()
	
public:	
	ATDSpawnVolumeActor();

	//*****************************************************
	//* ISaveGame
	virtual void LoadActor() override;
	virtual bool ShouldLoadTransform() override { return false; };
	//*****************************************************

	UPROPERTY(BlueprintReadOnly, SaveGame)
	bool bReached = false;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere)
	TArray<ATDTargetPoint_Spawn*> SpawnPoints;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> Box;
};
