#include "UI/WidgetController/TDWidgetControllerAttributeMenu.h"
#include "GAS/TDAttributeSet.h"
#include "GAS/Data/TDDA_Attribute.h"
#include "GameplayTags/TDGameplayTags.h"
#include "GAS/TDAbilitySystemComponent.h"
#include "Player/TDPlayerState.h"

void UTDWidgetControllerAttributeMenu::BroadcastInitialValues()
{
	UTDAttributeSet* AS = CastChecked<UTDAttributeSet>(AttributeSet);
	checkf(DataAttribute, TEXT("No DataAttribute. Check: UTDWidgetControllerAttributeMenu::BroadcastInitialValues()"));

	// UTDAttributeSet 내 TagsToAttributes 맵 변수의 GameplayTag를 순회하여 value들을 Broadcast시킨다.
	for (auto& Pair : AS->TagsToAttributes)
	{
		BroadcastAttributeInfo(Pair.Key, Pair.Value());
	}

	ATDPlayerState* TDPlayerState = CastChecked<ATDPlayerState>(PlayerState);
	AttributePointsChangedDelegate.Broadcast(TDPlayerState->GetAttributePoints());
}

void UTDWidgetControllerAttributeMenu::BindCallbacksToDependencies()
{
	UTDAttributeSet* AS = CastChecked<UTDAttributeSet>(AttributeSet);
	checkf(DataAttribute, TEXT("No DataAttribute. Check: UTDWidgetControllerAttributeMenu::BindCallbacksToDependencies()"));

	for (auto& Pair : AS->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
			[this, Pair](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeInfo(Pair.Key, Pair.Value());
			});

		ATDPlayerState* TDPlayerState = CastChecked<ATDPlayerState>(PlayerState);
		TDPlayerState->OnAttributePointsChangedDelegate.AddLambda([this](int32 Points)
			{
				AttributePointsChangedDelegate.Broadcast(Points);
			});
	}
}

void UTDWidgetControllerAttributeMenu::EnhanceAttribute(const FGameplayTag& AttributeTag)
{
	UTDAbilitySystemComponent* TDASC = CastChecked<UTDAbilitySystemComponent>(AbilitySystemComponent);
	checkf(TDASC, TEXT("No TDASC. Check: UTDWidgetControllerAttributeMenu::EnhanceAttribute"));

	TDASC->EnhanceAttribute(AttributeTag);
}

void UTDWidgetControllerAttributeMenu::BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const
{
	FDA_Attribute Info = DataAttribute->FindDA_AttributeForTag(AttributeTag);
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	DataAttributeInfoDelegate.Broadcast(Info);
}
