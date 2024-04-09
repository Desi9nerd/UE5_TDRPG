#pragma once
#include "CoreMinimal.h"
#include "UI/WidgetController/TDWidgetController.h"
#include "TDWidgetControllerAttributeMenu.generated.h"

class UTDDA_Attribute;
struct FTDDataAttributeInfo;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDataAttributeInfoSignature, const FTDDataAttributeInfo&, Info);
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class TDRPG_API UTDWidgetControllerAttributeMenu : public UTDWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FDataAttributeInfoSignature DataAttributeInfoDelegate;

protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UTDDA_Attribute> DataAttribute;

private:
	void BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const;
};
