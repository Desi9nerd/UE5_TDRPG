#pragma once
#include "CoreMinimal.h"
#include "UI/Widget/TDUserWidget.h"
#include "TDUW_AttributeMenu.generated.h"

class UTDWidgetController;
class UTDUW_Button;
class UTDUW_TextValueRow;
class UTDUW_TextValueRowButton;

/**
 * 
 */
UCLASS()
class TDRPG_API UTDUW_AttributeMenu : public UTDUserWidget
{
	GENERATED_BODY()

	
protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTDUW_Button> CloseButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTDUW_TextValueRowButton> Row_Strength;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTDUW_TextValueRowButton> Row_Intelligence;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTDUW_TextValueRowButton> Row_Resilience;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTDUW_TextValueRowButton> Row_Vigor;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTDUW_TextValueRow> Row_Armor;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTDUW_TextValueRow> Row_ArmorPenetration;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTDUW_TextValueRow> Row_BlockChance;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTDUW_TextValueRow> Row_CriticalHitChance;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTDUW_TextValueRow> Row_CriticalHitDamage;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTDUW_TextValueRow> Row_CriticalHitResistance;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTDUW_TextValueRow> Row_HealthRegeneration;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTDUW_TextValueRow> Row_ManaRegeneration;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTDUW_TextValueRow> Row_MaxHealth;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTDUW_TextValueRow> Row_MaxMana;

private:
	void SetAttributeTags();
	void BroadcastInitialValues(UTDWidgetController& InTaget);
	
};
