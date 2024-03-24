#pragma once
#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "TDAT_TargetData.generated.h"

/** ���콺 ��ġ �����͸� AbilityTask�� Ȱ���Ͽ� ����
 *  Owning GameplayAbility ���.
 *  Ŭ���̾�Ʈ(=locally controlled)��� SendMouseCursorData()�Լ��� ���Ͽ� TargetData�� ������ ����.
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMouseTargetDataSignature, const FGameplayAbilityTargetDataHandle&, DataHandle);

UCLASS()
class TDRPG_API UTDAT_TargetData : public UAbilityTask
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "TDAT_TargetDataUnderMouse", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility"))
	static UTDAT_TargetData* CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility);

	UPROPERTY(BlueprintAssignable)
	FMouseTargetDataSignature ValidData;

private:
	virtual void Activate() override;
	void SendMouseCursorData();
};
