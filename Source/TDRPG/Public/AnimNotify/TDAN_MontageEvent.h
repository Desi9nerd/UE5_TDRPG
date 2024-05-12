#pragma once
#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GameplayTagContainer.h"
#include "TDAN_MontageEvent.generated.h"

/**
 * 
 */
UCLASS()
class TDRPG_API UTDAN_MontageEvent : public UAnimNotify
{
	GENERATED_BODY()

public:
	UTDAN_MontageEvent();
	FString GetNotifyName_Implementation() const override;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag EventTag;

	UPROPERTY(EditAnywhere)
	float ComboAttackLevel;
};
