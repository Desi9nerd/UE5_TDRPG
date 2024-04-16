#include "Inventory/Item/Actors/TDItemActor.h"
#include "Net/UnrealNetwork.h"
#include "Engine/ActorChannel.h"
#include "Inventory/Item/TDItemInstance.h"

ATDItemActor::ATDItemActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATDItemActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATDItemActor, TDItemInstance);
}

bool ATDItemActor::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool bRenewed = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	
	// Channel�� ���� TDItemInstance�� ����. ��Ʈ��ũ�� ���� �ش� TDItemInstance�� ���°� Ŭ���̾�Ʈ�� ����ȭ�ǵ��� ��.
	bRenewed |= Channel->ReplicateSubobject(TDItemInstance, *Bunch, *RepFlags);

	return bRenewed;
}

void ATDItemActor::InitItemActor(UTDItemInstance* InTDItemInstance)
{
	TDItemInstance = InTDItemInstance;
	
}

void ATDItemActor::OnEquipped()
{
}

void ATDItemActor::OnUnequipped()
{
}

void ATDItemActor::OnDropped()
{
}

void ATDItemActor::OnRep_TDItemInstance(UTDItemInstance* OldTDItemInstance)
{
	InitItemActor(OldTDItemInstance);
}
