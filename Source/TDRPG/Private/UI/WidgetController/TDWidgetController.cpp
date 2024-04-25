#include "UI/WidgetController/TDWidgetController.h"

#include "GAS/TDAbilitySystemComponent.h"
#include "GAS/TDAttributeSet.h"
#include "GAS/Data/TDDA_Ability.h"
#include "Player/TDPlayerController.h"
#include "Player/TDPlayerState.h"

void UTDWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController;
	PlayerState = WCParams.PlayerState;
	ASC = WCParams.AbilitySystemComponent;
	AttributeSet = WCParams.AttributeSet;
}

void UTDWidgetController::BroadcastInitialValues()
{
}

void UTDWidgetController::BindCallbacksToDependencies()
{
}

void UTDWidgetController::BroadcastDA_AbilityInfo()
{
	if (false == GetTDASC()->bStartGivenASC) return; // �̹� ����Ǿ��ٸ� �ٽ� �������� �ʰ� ����.

	// ������ TDASC ������ ��ε�ĳ����.
	// BroadcastDelegate ���� �� ���ٷ� ���ε�.
	FForEachAbility BroadcastDelegate;
	BroadcastDelegate.BindLambda([this](const FGameplayAbilitySpec& AbilitySpec)
		{
			FDA_Ability DA_AbilityInfo = TDDA_Ability->FindDA_AbilityForTag(GetTDASC()->GetAbilityTagFromSpec(AbilitySpec));
			DA_AbilityInfo.InputTag = GetTDASC()->GetInputTagFromSpec(AbilitySpec); // � InputTag�� � AbilitySpec�� ��Ī�Ǵ��� �˱�����
			DA_AbilityInfo.StatusTag = GetTDASC()->GetStatusTagFromSpec(AbilitySpec);
			DA_AbilityInfoDelegate.Broadcast(DA_AbilityInfo);
		});

	GetTDASC()->ForEachAbility(BroadcastDelegate); // ��������Ʈ ������
}

TObjectPtr<APlayerController> UTDWidgetController::GetPlayerController()
{
	return PlayerController;
}

TObjectPtr<ATDPlayerController> UTDWidgetController::GetTDPlayerController()
{
	//TDPlayerController = IsValid(TDPlayerController) ? TDPlayerController : Cast<ATDPlayerController>(PlayerController);
	if (false == IsValid(TDPlayerController))
	{
		TDPlayerController = Cast<ATDPlayerController>(PlayerController);
	}

	return TDPlayerController;
}

TObjectPtr<APlayerState> UTDWidgetController::GetPlayerState()
{
	return PlayerState;
}

TObjectPtr<ATDPlayerState> UTDWidgetController::GetTDPlayerState()
{
	if (false == IsValid(TDPlayerState))
	{
		TDPlayerState = Cast<ATDPlayerState>(PlayerState);
	}

	return TDPlayerState;
}

TObjectPtr<UAbilitySystemComponent> UTDWidgetController::GetASC()
{
	return ASC;
}

TObjectPtr<UTDAbilitySystemComponent> UTDWidgetController::GetTDASC()
{
	if (false == IsValid(TDASC))
	{
		TDASC = Cast<UTDAbilitySystemComponent>(ASC);
	}

	return TDASC;
}

TObjectPtr<UAttributeSet> UTDWidgetController::GetAttributeSet()
{
	return AttributeSet;
}

TObjectPtr<UTDAttributeSet> UTDWidgetController::GetTDAttributeSet()
{
	if (false == IsValid(TDAttributeSet))
	{
		TDAttributeSet = Cast<UTDAttributeSet>(AttributeSet);
	}

	return TDAttributeSet;
}
