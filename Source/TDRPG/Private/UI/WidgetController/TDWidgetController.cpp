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
	if (false == GetTDASC()->bStartGivenASC) return; // 이미 적용되었다면 다시 적용하지 않고 리턴.

	// 위젯에 TDASC 데이터 브로드캐스팅.
	// BroadcastDelegate 생성 후 람다로 바인딩.
	FForEachAbility BroadcastDelegate;
	BroadcastDelegate.BindLambda([this](const FGameplayAbilitySpec& AbilitySpec)
		{
			FDA_Ability DA_AbilityInfo = TDDA_Ability->FindDA_AbilityForTag(GetTDASC()->GetAbilityTagFromSpec(AbilitySpec));
			DA_AbilityInfo.InputTag = GetTDASC()->GetInputTagFromSpec(AbilitySpec); // 어떤 InputTag와 어떤 AbilitySpec이 매칭되는지 알기위해
			DA_AbilityInfo.StatusTag = GetTDASC()->GetStatusTagFromSpec(AbilitySpec);
			DA_AbilityInfoDelegate.Broadcast(DA_AbilityInfo);
		});

	GetTDASC()->ForEachAbility(BroadcastDelegate); // 델리게이트 보내기
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
