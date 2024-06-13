#include "GAS/Data/TDDA_DropItem.h"

TArray<FLootItem> UTDDA_DropItem::GetLootItems()
{
	TArray<FLootItem> DropItemList;

	for (FLootItem& Item : LootItems)
	{
		for (int32 i = 0; i < Item.MaxNumberToSpawn; ++i)
		{
			if (FMath::FRandRange(1.f, 100.f) < Item.ChanceToSpawn)
			{
				FLootItem NewItem;
				NewItem.LootClass = Item.LootClass;
				NewItem.bLootLevelOverride = Item.bLootLevelOverride;
				DropItemList.Add(NewItem);
			}
		}
	}

	return DropItemList;
}
