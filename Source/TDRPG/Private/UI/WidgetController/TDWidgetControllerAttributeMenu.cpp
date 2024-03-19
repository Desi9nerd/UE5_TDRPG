#include "UI/WidgetController/TDWidgetControllerAttributeMenu.h"
#include "GAS/TDAttributeSet.h"
#include "GAS/Data/TDDataAttribute.h"
#include "GameplayTags/TDGameplayTags.h"

void UTDWidgetControllerAttributeMenu::BroadcastInitialValues()
{
	UTDAttributeSet* AS = CastChecked<UTDAttributeSet>(AttributeSet);

	check(DataAttribute);

	FTDDataAttributeInfo Info = DataAttribute->FindAttributeInfoForTag(FTDGameplayTags::GetTDGameplayTags().Attributes_Stat_Strength);
	Info.AttributeValue = AS->GetStrength();
	DataAttributeInfoDelegate.Broadcast(Info);

}

void UTDWidgetControllerAttributeMenu::BindCallbacksToDependencies()
{
}
