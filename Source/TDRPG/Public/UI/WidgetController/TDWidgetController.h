#pragma once
#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "UObject/NoExportTypes.h"
#include "TDWidgetController.generated.h"

class UTDDA_Ability;
class UAttributeSet;
class UTDAttributeSet;
class UAbilitySystemComponent;
class UTDAbilitySystemComponent;
class ATDPlayerController;
class ATDPlayerState;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChangedSignature, int32, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDA_AbilitySignature, const FDA_Ability&, DA_AbilityInfo);

/** TDWidgetController는 위젯과 데이터를 관리하는 클래스 사이의 교량역할
 *  위젯의 변경사항(ex.버튼클릭)을 클래스에 알려주는 역할
 */

USTRUCT(BlueprintType)
struct FWidgetControllerParams
{
	GENERATED_BODY()

	FWidgetControllerParams() {}
	FWidgetControllerParams(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
		: PlayerController(PC), PlayerState(PS), AbilitySystemComponent(ASC), AttributeSet(AS) {} // 멤버 초기화 리스트

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerController> PlayerController = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerState> PlayerState = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAttributeSet> AttributeSet = nullptr;
};

UCLASS()
class TDRPG_API UTDWidgetController : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable) // BP에서 호출가능
	void SetWidgetControllerParams(const FWidgetControllerParams& WCParams);

	UFUNCTION(BlueprintCallable)
	virtual void BroadcastInitialValues();
	virtual void BindCallbacksToDependencies();
	void BroadcastDA_AbilityInfo();

	UPROPERTY(BlueprintAssignable, Category = "GAS|Messages")
	FDA_AbilitySignature DA_AbilityInfoDelegate;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget Data")
	TObjectPtr<UTDDA_Ability> TDDA_Ability;

	TObjectPtr<APlayerController> GetPlayerController();
	TObjectPtr<ATDPlayerController> GetTDPlayerController();
	TObjectPtr<APlayerState> GetPlayerState();
	TObjectPtr<ATDPlayerState> GetTDPlayerState();
	TObjectPtr<UAbilitySystemComponent> GetASC();
	TObjectPtr<UTDAbilitySystemComponent> GetTDASC();
	TObjectPtr<UAttributeSet> GetAttributeSet();
	TObjectPtr<UTDAttributeSet> GetTDAttributeSet();

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<APlayerController> PlayerController;
	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<ATDPlayerController> TDPlayerController;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<APlayerState> PlayerState;
	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<ATDPlayerState> TDPlayerState;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UAbilitySystemComponent> ASC;
	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UTDAbilitySystemComponent> TDASC;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UAttributeSet> AttributeSet;
	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UTDAttributeSet> TDAttributeSet;
};
