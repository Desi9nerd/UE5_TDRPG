#pragma once
#include "CoreMinimal.h"
#include "UI/WidgetController/TDWidgetController.h"
#include "TDWidgetControllerOverlay.generated.h"

class UTDUserWidget;

/** UTDWidgetController 상속을 받는 클래스
 * 
 */

USTRUCT(BlueprintType)
struct FUIWidgetRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag MessageTag = FGameplayTag();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Message = FText();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UTDUserWidget> MessageWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* Image = nullptr;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageWidgetRowSignature, FUIWidgetRow, Row);

UCLASS(BlueprintType, Blueprintable)
class TDRPG_API UTDWidgetControllerOverlay : public UTDWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable, Category = "GAS: Attributes")
	FOnAttributeChangedSignature OnHealthChanged;
	UPROPERTY(BlueprintAssignable, Category = "GAS: Attributes")
	FOnAttributeChangedSignature OnMaxHealthChanged;
	UPROPERTY(BlueprintAssignable, Category = "GAS: Attributes")
	FOnAttributeChangedSignature OnManaChanged;
	UPROPERTY(BlueprintAssignable, Category = "GAS: Attributes")
	FOnAttributeChangedSignature OnMaxManaChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Messages")
	FMessageWidgetRowSignature MessageWidgetRowDelegate;


protected:
	void HealthChanged(const FOnAttributeChangeData& Data) const;
	void MaxHealthChanged(const FOnAttributeChangeData& Data) const;
	void ManaChanged(const FOnAttributeChangeData& Data) const;
	void MaxManaChanged(const FOnAttributeChangeData& Data) const;
	void ReadDataTableRowByTag(const FGameplayTagContainer& AssetTags);

	template<typename T> // DataTable 읽기용
	T* GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget Data")
	TObjectPtr<UDataTable> MessageWidgetDataTable;
};

template <typename T>
T* UTDWidgetControllerOverlay::GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag)
{
	return DataTable->FindRow<T>(Tag.GetTagName(), TEXT(""));
}
