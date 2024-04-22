#include "UI/WidgetController/TDWidgetControllerAttributeMenu.h"
#include "GAS/TDAttributeSet.h"
#include "GAS/Data/TDDA_Attribute.h"
#include "GameplayTags/TDGameplayTags.h"
#include "GAS/TDAbilitySystemComponent.h"
#include "Player/TDPlayerState.h"

void UTDWidgetControllerAttributeMenu::BroadcastInitialValues()
{
	checkf(DataAttribute, TEXT("No DataAttribute. Check: UTDWidgetControllerAttributeMenu::BroadcastInitialValues()"));

	// UTDAttributeSet 내 TagsToAttributes 맵 변수의 GameplayTag를 순회하여 value들을 Broadcast시킨다.
	for (auto& Pair : GetTDAttributeSet()->TagsToAttributes)
	{
		BroadcastAttributeInfo(Pair.Key, Pair.Value());
	}

	AttributePointsChangedDelegate.Broadcast(GetTDPlayerState()->GetAttributePoints());
}

void UTDWidgetControllerAttributeMenu::BindCallbacksToDependencies()
{
	checkf(DataAttribute, TEXT("No DataAttribute. Check: UTDWidgetControllerAttributeMenu::BindCallbacksToDependencies()"));

	for (auto& Pair : GetTDAttributeSet()->TagsToAttributes)
	{
		GetASC()->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
			[this, Pair](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeInfo(Pair.Key, Pair.Value());
			});
		
		GetTDPlayerState()->OnAttributePointsChangedDelegate.AddLambda([this](int32 Points)
			{
				AttributePointsChangedDelegate.Broadcast(Points);
			});
	}
}

void UTDWidgetControllerAttributeMenu::EnhanceAttribute(const FGameplayTag& AttributeTag)
{
	checkf(GetTDASC(), TEXT("No TDASC. Check: UTDWidgetControllerAttributeMenu::EnhanceAttribute"));

	GetTDASC()->EnhanceAttribute(AttributeTag);
}

void UTDWidgetControllerAttributeMenu::BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute)
{
	FDA_Attribute Info = DataAttribute->FindDA_AttributeForTag(AttributeTag);
	Info.AttributeValue = Attribute.GetNumericValue(GetAttributeSet());
	DataAttributeInfoDelegate.Broadcast(Info);
}
