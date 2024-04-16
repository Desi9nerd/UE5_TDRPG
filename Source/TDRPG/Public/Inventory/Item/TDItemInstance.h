#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TDItemInstance.generated.h"

class UTDItemStaticData;
class ATDItemActor;

/**
 * 
 */

UCLASS(BlueprintType, Blueprintable)
class TDRPG_API UTDItemInstance : public UObject
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const;
	virtual void Init(TSubclassOf<UTDItemStaticData> InTDItemStaticDataClass);
	virtual bool IsSupportedForNetworking() const override;
	virtual void OnEquipped(AActor* InOwner);
	virtual void OnUnequipped(AActor* InOwner);
	virtual void OnDropped(AActor* InOwner);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	const UTDItemStaticData* GetTDItemStaticData() const;

	UPROPERTY(Replicated)
	TSubclassOf<UTDItemStaticData> TDItemStaticDataClass;

protected:
	UPROPERTY(ReplicatedUsing = OnRep_Equipped)
	bool bEquipped = false;
	UFUNCTION()
	void OnRep_Equipped();

	UPROPERTY(Replicated)
	ATDItemActor* ItemActor;

};
