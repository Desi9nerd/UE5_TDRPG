#pragma once
#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "TDMVVM_StartMenu.generated.h"

class UTDMVVM_Slot;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSlotSelected);

/** 
 *  게임정보가 저장된 슬롯 3개 등록.
 */

UCLASS()
class TDRPG_API UTDMVVM_StartMenu : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	void InitializeLoadSlots();

	UPROPERTY(BlueprintAssignable)
	FSlotSelected SlotSelected;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UTDMVVM_Slot> LoadSlotViewModelClass;

	UFUNCTION(BlueprintPure)
	UTDMVVM_Slot* GetLoadSlotViewModelByIndex(int32 Index) const;

	UFUNCTION(BlueprintCallable)
	void NewSlotButtonPressed(int32 Slot, const FString& EnteredName);

	UFUNCTION(BlueprintCallable)
	void NewGameButtonPressed(int32 Slot);

	UFUNCTION(BlueprintCallable)
	void SelectSlotButtonPressed(int32 Slot);

	void LoadData();

private:
	UPROPERTY()
	TMap<int32, UTDMVVM_Slot*> LoadSlots;

	UPROPERTY()
	TObjectPtr<UTDMVVM_Slot> LoadSlot_0;
	UPROPERTY()
	TObjectPtr<UTDMVVM_Slot> LoadSlot_1;
	UPROPERTY()
	TObjectPtr<UTDMVVM_Slot> LoadSlot_2;

};
