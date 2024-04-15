#include "Inventory/Item/TDItemInstance.h"
#include "Net/UnrealNetwork.h"

void UTDItemInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UTDItemInstance, TDItemStaticDataClass);
	DOREPLIFETIME(UTDItemInstance, bEquipped);
}

void UTDItemInstance::Init(TSubclassOf<UTDItemStaticData> InTDItemStaticDataClass)
{
	TDItemStaticDataClass = InTDItemStaticDataClass;
}

bool UTDItemInstance::IsSupportedForNetworking() const
{
	return true;
}

void UTDItemInstance::OnEquipped()
{
	bEquipped = true;
}

void UTDItemInstance::OnUnequipped()
{
	bEquipped = false;
}

void UTDItemInstance::OnRep_Equipped()
{
	bEquipped = true;
}
