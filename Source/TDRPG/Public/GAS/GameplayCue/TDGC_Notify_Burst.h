#pragma once
#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "TDGC_Notify_Burst.generated.h"

/**
 * 
 */
UCLASS()
class TDRPG_API UTDGC_Notify_Burst : public UGameplayCueNotify_Static
{
	GENERATED_BODY()

public:
	UTDGC_Notify_Burst();

	virtual bool OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const override;

private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess))
	TObjectPtr<USoundBase> SoundClass;
};
