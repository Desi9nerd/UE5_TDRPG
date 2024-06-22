#pragma once
#include "CoreMinimal.h"
#include "Library/TDItemLibrary.h"
#include "UI/Widget/TDUW.h"
#include "TDUW_InventoryPanel.generated.h"

class UTDInventoryComponent;
class UTileView;
class ATDCharacter;
class UHorizontalBox;
class UButton;
class UListView;
class UVerticalBox;
class UGridPanel;
class UTDUW_InventorySlot;

UCLASS()
class TDRPG_API UTDUW_InventoryPanel : public UTDUW
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	void CreateInventorySlotWidgets();
	//void UpdateCategoryItems(TArray<UTDInventorySlot*> CategoryItems, int32 AmountOfSlots);

	void DisplayInventorySlotWidgets();
	void DisplayAllItemsWidgets();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TObjectPtr<UTDInventoryComponent> TDInventoryComponent;

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
	UFUNCTION()
	void OnAllButtonClicked();

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
	TObjectPtr<UButton> Button_Category_All;
	
	UPROPERTY(meta = (BindWidgetOptional), meta = (AllowPrivateAccess))
	TObjectPtr<UTileView> TileView_Inventory;

	UPROPERTY()
	TObjectPtr<ATDCharacter> TDCharacter;

	int32 TotalNumOfSlots;
};
