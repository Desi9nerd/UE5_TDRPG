#include "UI/WidgetController/TDWidgetControllerAttributeMenu.h"
#include "GAS/TDAttributeSet.h"
#include "GAS/Data/TDDA_Attribute.h"
#include "GameplayTags/TDGameplayTags.h"

void UTDWidgetControllerAttributeMenu::BroadcastInitialValues()
{
	UTDAttributeSet* AS = CastChecked<UTDAttributeSet>(AttributeSet);
	check(DataAttribute);

	// UTDAttributeSet 내 TagsToAttributes 맵 변수의 GameplayTag를 순회하여 value들을 Broadcast시킨다.
	for (auto& Pair : AS->TagsToAttributes)
	{
		BroadcastAttributeInfo(Pair.Key, Pair.Value());
	}
}

void UTDWidgetControllerAttributeMenu::BindCallbacksToDependencies()
{
	UTDAttributeSet* AS = CastChecked<UTDAttributeSet>(AttributeSet);
	check(DataAttribute);

	for (auto& Pair : AS->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
			[this, Pair](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeInfo(Pair.Key, Pair.Value());
			}
		);
	}
}

void UTDWidgetControllerAttributeMenu::BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const
{
	FTDDataAttributeInfo Info = DataAttribute->FindAttributeInfoForTag(AttributeTag);
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	DataAttributeInfoDelegate.Broadcast(Info);
}
