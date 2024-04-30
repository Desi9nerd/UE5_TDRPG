#include "UI/Widget/Inventory/TDUW_InventoryCategory.h"
#include "Components/Image.h"

void UTDUW_InventoryCategory::NativeConstruct()
{
	Image_Category->SetBrushFromTexture(CategoryIcon);
}

