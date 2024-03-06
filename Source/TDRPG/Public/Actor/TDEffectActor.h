#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TDEffectActor.generated.h"

class UGameplayEffect;

UCLASS()
class TDRPG_API ATDEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ATDEffectActor();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable) // BP에서 호출가능
	void ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	TSubclassOf<UGameplayEffect> InstantGameplayEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	TSubclassOf<UGameplayEffect> DurationGameplayEffectClass;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> Mesh;
};
