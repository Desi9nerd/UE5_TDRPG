#pragma once
#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "SaveGame/TDSaveGame_Load.h"
#include "TDMVVM_Slot.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSetWidgetSwitcherIndex, int32, WidgetSwitcherIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEnableSelectSlotButton, bool, bEnable);

/** 플레이한 캐릭터 이름과 게임 정보를 저장하는 슬롯.
 * 
 */

UCLASS()
class TDRPG_API UTDMVVM_Slot : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	void InitializeSlot();
	FString GetPlayerName() const { return PlayerName; }
	int32 GetPlayerLevel() const { return PlayerLevel; }
	FString GetMapName() const { return MapName; }
	FString GetLoadSlotName() const { return LoadSlotName; }
	void SetPlayerName(FString InPlayerName);
	void SetPlayerLevel(int32 InPlayerLevel);
	void SetMapName(FString InLevelMapName);
	void SetLoadSlotName(FString InLoadSlotName);

	UPROPERTY(BlueprintAssignable)
	FSetWidgetSwitcherIndex SetWidgetSwitcherIdx;
	UPROPERTY(BlueprintAssignable)
	FEnableSelectSlotButton EnableSelectSlotButton;


	UPROPERTY()
	int32 SlotIndex;

	UPROPERTY() // TDMVVM_StartMenu의 LoadData()에서 설정.
	TEnumAsByte<ESG_SaveSlotStatus> SlotStatus; // 슬롯상태.

	UPROPERTY()
	FName PlayerStartTag;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess = "true"))
	FString PlayerName; // 플레이어 이름. Field Notifies.

	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess = "true"));
	int32 PlayerLevel; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess = "true"))
	FString MapName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess = "true"))
	FString LoadSlotName;
};
