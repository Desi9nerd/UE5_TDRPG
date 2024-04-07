#pragma once
#include "CoreMinimal.h"
#include "GAS/GameplayAbility/TDGA.h"
#include "TDGA_Summon.generated.h"

/** ĳ���� �ֺ��� �̴Ͼ���� ��ȯ�� ���� �����ϴ� GameplayAbility
 * 
 */
UCLASS()
class TDRPG_API UTDGA_Summon : public UTDGA
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	TArray<FVector> GetSpawnLocations();

	UPROPERTY(EditDefaultsOnly, Category = "Summon")
	int32 NumMinions = 5;

	UPROPERTY(EditDefaultsOnly, Category = "Summon")
	TArray<TSubclassOf<APawn>> MinionClasses;

	UPROPERTY(EditDefaultsOnly, Category = "Summon")
	float MinSpawnDistance = 150.f;
	UPROPERTY(EditDefaultsOnly, Category = "Summon")
	float MaxSpawnDistance = 400.f;
	UPROPERTY(EditDefaultsOnly, Category = "Summon")
	float SpawnSpread = 90.f;
};
