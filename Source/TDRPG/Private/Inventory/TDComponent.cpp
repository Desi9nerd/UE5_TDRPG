#include "Inventory/TDComponent.h"

UTDComponent::UTDComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

UDataTable* UTDComponent::GetDataTable(uint8 InItemID)
{
	if (IsValid(ItemDataTable))
	{
		TArray<FName> RowNames = ItemDataTable->GetRowNames();

		for (int32 i = 0; i < RowNames.Num(); i++)
		{
			if (RowNames[i] == FName(FString::FromInt(InItemID)))
			{
				
			}
		}
	}
}
