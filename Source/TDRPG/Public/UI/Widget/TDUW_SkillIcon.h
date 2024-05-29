#pragma once
#include "CoreMinimal.h"
#include "GAS/BPAsyncTask/TDBPAsync_WaitCooldown.h"
#include "UI/Widget/TDUW.h"
#include "TDUW_SkillIcon.generated.h"

class USizeBox;
class UTextBlock;
class UImage;
class UButton;

/**
 * 
 */

UCLASS()
class TDRPG_API UTDUW_SkillIcon : public UTDUW
{
	GENERATED_BODY()

public:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	USizeBox* SizeBox_Root;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* Image_Background;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* Image_SkillIcon;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* Image_Ring;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* Text_Cooldown;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill Icon")
	float BoxWidth = 60.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill Icon")
	float BoxHeight = 60.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill Icon")
	FSlateBrush BackgroundBrush;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill Icon")
	FSlateBrush RingBrush;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill Icon")
	FSlateBrush SkillIconBrush;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill Icon")
	FSlateBrush TransparentBrush;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill Icon")
	float Padding_Background;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill Icon")
	float Padding_SkillIcon;

private:
	void ReceiveDA_Ability(FDA_Ability InDA_Ability);
	void SetCooldownState();

	UFUNCTION()
	void OnAbilityInfoDelegate_Event(const FDA_Ability& DA_Ability);

	UFUNCTION()
	void OnUpdateTimer();

	FGameplayTag InputTag;
	FGameplayTag CooldownTag;
	TObjectPtr<UTDBPAsync_WaitCooldown> AsyncTask_WaitCooldown;
	float CooldownTint;
	float TimeRemaining;
	float TimerFrequency = 0.1f;
	FTimerHandle CooldownTimerHandle;
};
