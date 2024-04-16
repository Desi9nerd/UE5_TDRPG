#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TDItemInstance.generated.h"

class UTDItemStaticData;

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
	virtual void OnEquipped();
	virtual void OnUnequipped();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	const UTDItemStaticData* GetTDItemStaticData() const;

	UPROPERTY(Replicated)
	TSubclassOf<UTDItemStaticData> TDItemStaticDataClass;

	UPROPERTY(ReplicatedUsing = OnRep_Equipped)
	bool bEquipped = false;

	UFUNCTION()
	void OnRep_Equipped();

};
