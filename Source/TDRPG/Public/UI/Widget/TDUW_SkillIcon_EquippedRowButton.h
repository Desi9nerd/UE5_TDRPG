#pragma once
#include "CoreMinimal.h"
#include "UI/Widget/TDUW_SkillIcon.h"
#include "TDUW_SkillIcon_EquippedRowButton.generated.h"

/**
 * 
 */
UCLASS()
class TDRPG_API UTDUW_SkillIcon_EquippedRowButton : public UTDUW_SkillIcon
{
	GENERATED_BODY()

public:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_ClickHover;


private:
	UFUNCTION()
	void OnButtonClicked();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess), Category = "SkillIcon_EquippedRowButton")
	FGameplayTag TDInputTag;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess), Category = "SkillIcon_EquippedRowButton")
	FGameplayTag TDAbilityTypeTag;
};
