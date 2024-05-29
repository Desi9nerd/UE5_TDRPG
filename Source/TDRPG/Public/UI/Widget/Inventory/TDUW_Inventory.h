#pragma once
#include "CoreMinimal.h"
#include "Library/TDItemLibrary.h"
#include "UI/Widget/TDUW.h"
#include "TDUW_Inventory.generated.h"

class ATDCharacter;
class UHorizontalBox;
class UButton;
class UListView;
class UVerticalBox;
class UGridPanel;
class UTDUW_InventorySlot;

UCLASS()
class TDRPG_API UTDUW_Inventory : public UTDUW
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	void CreateInventorySlotWidgets();
	void UpdateCategoryItems(TArray<FInventorySlot>* CategoryItems, int32 AmountOfSlots);

	void DisplayInventorySlotWidgets();

	int32 DraggedSlotIndex;

private:
	UFUNCTION()
	void OnWeaponButtonClicked();
	UFUNCTION()
	void OnArmorButtonClicked();
	UFUNCTION()
	void OnPotionButtonClicked();
	UFUNCTION()
	void OnFoodButtonClicked();

	UPROPERTY(meta = (BindWidgetOptional), meta = (AllowPrivateAccess))
	TObjectPtr<UHorizontalBox> HB_Categories;

	UPROPERTY(meta = (BindWidgetOptional), meta = (AllowPrivateAccess))
	TObjectPtr<UButton> Button_Category_Weapon;
	UPROPERTY(meta = (BindWidgetOptional), meta = (AllowPrivateAccess))
	TObjectPtr<UButton> Button_Category_Armor;
	UPROPERTY(meta = (BindWidgetOptional), meta = (AllowPrivateAccess))
	TObjectPtr<UButton> Button_Category_Potion;
	UPROPERTY(meta = (BindWidgetOptional), meta = (AllowPrivateAccess))
	TObjectPtr<UButton> Button_Category_Food;

	UPROPERTY(meta = (BindWidgetOptional), meta = (AllowPrivateAccess))
	TObjectPtr<UGridPanel> Grid_Inventory;

	
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess))
	TSubclassOf<UTDUW_InventorySlot> InventorySlotWidgetClass;

	UPROPERTY()
	TObjectPtr<ATDCharacter> TDCharacter;

};
