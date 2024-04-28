#pragma once
#include "CoreMinimal.h"
#include "UI/Widget/TDUW_SkillIcon.h"
#include "TDUW_SkillIcon_Button.generated.h"

/**
 * 
 */
UCLASS()
class TDRPG_API UTDUW_SkillIcon_Button : public UTDUW_SkillIcon
{
	GENERATED_BODY()

public:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SkillIcon_Button AbilityTag")
	FGameplayTag TDAbilityTag;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_Selection; // 선택 시 테두리 

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_ClickHover;

private:
	UFUNCTION()
	void OnAbilityInfoDelegate_Event2(const FDA_Ability& InDA_Ability);
	UFUNCTION()
	void OnSkillIconReassignedDelegate_Event(const FGameplayTag& InAbilityTag);
	UFUNCTION()
	void OnButtonClicked();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess), Category = "SkillIcon_Button")
	TObjectPtr<UMaterialInterface> MI_Locked;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess), Category = "SkillIcon_Button")
	TObjectPtr<UMaterialInterface> MI_EquippedUnlocked;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess), Category = "SkillIcon_Button")
	TObjectPtr<UTexture2D> Texture_Locked;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess), Category = "SkillIcon_Button")
	TObjectPtr<UTexture2D> Texture_EquippedUnlocked;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess), Category = "SkillIcon_Button")
	TObjectPtr<USoundBase> Sound_Click;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess), Category = "SkillIcon_Button")
	TObjectPtr<USoundBase> Sound_Cancel;

	bool bSelected = false;
};
