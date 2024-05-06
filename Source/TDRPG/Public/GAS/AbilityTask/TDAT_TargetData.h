#pragma once
#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "TDAT_TargetData.generated.h"

/** 마우스 위치 데이터를 AbilityTask를 활용하여 보냄
 *  Owning GameplayAbility 사용.
 *  클라이언트(=locally controlled)라면 SendMouseCursorData()함수를 콜하여 TargetData를 서버에 보냄.
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMouseTargetDataSignature, const FGameplayAbilityTargetDataHandle&, DataHandle);

UCLASS()
class TDRPG_API UTDAT_TargetData : public UAbilityTask
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "TDAT_TargetDataUnderMouse", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility"))
	static UTDAT_TargetData* CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility);
	virtual void Activate() override;

	UPROPERTY(BlueprintAssignable)
	FMouseTargetDataSignature ValidData;

	UPROPERTY()
	FVector_NetQuantize MouseCursorLocation;

private:
	void SendMouseCursorData(); // Client. 마우스 커서 데이터 보내기
	void OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag); // Server.
};
