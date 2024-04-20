#include "Inventory/Item/TDItemInstance.h"
#include "Net/UnrealNetwork.h"

void UTDItemInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);
	
}

bool UTDItemInstance::IsSupportedForNetworking() const
{
	return true;
}
