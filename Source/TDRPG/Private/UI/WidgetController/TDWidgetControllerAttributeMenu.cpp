#include "UI/WidgetController/TDWidgetControllerAttributeMenu.h"
#include "GAS/TDAttributeSet.h"
#include "GAS/Data/TDDataAttribute.h"
#include "GameplayTags/TDGameplayTags.h"

void UTDWidgetControllerAttributeMenu::BroadcastInitialValues()
{
	UTDAttributeSet* AS = CastChecked<UTDAttributeSet>(AttributeSet);

	check(DataAttribute);

	// UTDAttributeSet 내 TagsToAttributes 맵 변수의 GameplayTag를 순회하여 value들을 Broadcast시킨다.
	for (auto& Pair : AS->TagsToAttributes)
	{
		FTDDataAttributeInfo Info = DataAttribute->FindAttributeInfoForTag(Pair.Key);
		Info.AttributeValue = Pair.Value.Execute().GetNumericValue(AS);
		DataAttributeInfoDelegate.Broadcast(Info);
	}
}

void UTDWidgetControllerAttributeMenu::BindCallbacksToDependencies()
{
}
