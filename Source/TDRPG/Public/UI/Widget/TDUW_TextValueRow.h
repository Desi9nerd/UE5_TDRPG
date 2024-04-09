#pragma once
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UI/Widget/TDUW.h"
#include "TDUW_TextValueRow.generated.h"

class USizeBox;
class UTextBlock;
class UTDUW_FrameValue;

/**
 * 
 */
UCLASS()
class TDRPG_API UTDUW_TextValueRow : public UTDUW
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	USizeBox* SizeBox_Root;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextBlock_Label;
	UPROPERTY(meta = (BindWidget))
	UTDUW_FrameValue* FramedValue;

	UPROPERTY()
	FGameplayTag AttributeTag;

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	UTDUW_TextValueRow();

private:
	void UpdateFrameSize();
	void SetLabelText(FText InLabelText);
	void SetNumericalValueInt(int32 NewValue);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta =(AllowPrivateAccess), Category = "Row Properties")
	float BoxWidth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta =(AllowPrivateAccess), Category = "Row Properties")
	float BoxHeight;
};
