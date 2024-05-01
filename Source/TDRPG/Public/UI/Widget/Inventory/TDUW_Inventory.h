#pragma once
#include "CoreMinimal.h"
#include "UI/Widget/TDUW.h"
#include "TDUW_Inventory.generated.h"

class ATDCharacter;
class UHorizontalBox;
class UButton;
class UListView;
class UVerticalBox;
class UGridPanel;
class UTDUW_InventorySlot;

/**
 * 
 */

UCLASS()
class TDRPG_API UTDUW_Inventory : public UTDUW
{
	GENERATED_BODY()

public:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	void Set(int32 ShowCategoryID);
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	void CreateInventorySlotWidgets();

private:
	UFUNCTION()
	void OnWeaponButtonClicked();
	UFUNCTION()
	void OnArmorButtonClicked();

	UPROPERTY(meta = (BindWidgetOptional), meta = (AllowPrivateAccess))
	TObjectPtr<UHorizontalBox> HB_Categories;

	UPROPERTY(meta = (BindWidgetOptional), meta = (AllowPrivateAccess))
	TObjectPtr<UButton> Button_Category_Weapon;
	UPROPERTY(meta = (BindWidgetOptional), meta = (AllowPrivateAccess))
	TObjectPtr<UButton> Button_Category_Armor;

	UPROPERTY(meta = (BindWidgetOptional), meta = (AllowPrivateAccess))
	TObjectPtr<UGridPanel> Grid_Inventory;

	// Test용
	TObjectPtr<UListView> VerticalListView;
	
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess))
	TSubclassOf<UTDUW_InventorySlot> InventorySlotWidgetClass;

	UPROPERTY()
	TObjectPtr<ATDCharacter> TDCharacter;
};
