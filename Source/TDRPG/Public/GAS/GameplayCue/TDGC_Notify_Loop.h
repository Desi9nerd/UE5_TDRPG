#pragma once
#include "CoreMinimal.h"
#include "GameplayCueNotify_Actor.h"
#include "TDGC_Notify_Loop.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;
/**
 * 
 */
UCLASS()
class TDRPG_API ATDGC_Notify_Loop : public AGameplayCueNotify_Actor
{
	GENERATED_BODY()

public:
	ATDGC_Notify_Loop();

	virtual bool WhileActive_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) override;
	virtual bool OnRemove_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) override;

private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowAbstract))
	TObjectPtr<UNiagaraSystem> SystemTemplate;

	UPROPERTY()
	TObjectPtr<UNiagaraComponent> BeamSystem;

	UPROPERTY(EditDefaultsOnly, meta = (AllowAbstract))
	FName SocketName = FName(TEXT("TipSocket"));

	UPROPERTY(EditDefaultsOnly, meta = (AllowAbstract))
	FString BeamEnd = FString("Beam End"); // NiagaraSystem 내에 선언한 변수명과 일치시켜야 함. 다른 에셋 사용시 변수 생성 후 변수명을 Beam End로 만들어주어야 한다.

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAudioComponent> LoopSound;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USoundBase> LoopingSound;
};
