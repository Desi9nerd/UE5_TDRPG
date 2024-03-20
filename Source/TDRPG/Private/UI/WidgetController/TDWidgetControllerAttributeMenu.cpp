#include "UI/WidgetController/TDWidgetControllerAttributeMenu.h"
#include "GAS/TDAttributeSet.h"
#include "GAS/Data/TDDataAttribute.h"
#include "GameplayTags/TDGameplayTags.h"

void UTDWidgetControllerAttributeMenu::BroadcastInitialValues()
{
	UTDAttributeSet* AS = CastChecked<UTDAttributeSet>(AttributeSet);

	check(DataAttribute);

	// UTDAttributeSet �� TagsToAttributes �� ������ GameplayTag�� ��ȸ�Ͽ� value���� Broadcast��Ų��.
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
