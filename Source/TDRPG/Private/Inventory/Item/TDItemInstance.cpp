#include "Inventory/Item/TDItemInstance.h"
#include "Character/TDCharacter.h"
#include "GAS/TDAbilitySystemBPLibrary.h"
#include "Inventory/Item/TDItemStaticData.h"
#include "Inventory/Item/Actors/TDItemActor.h"
#include "Net/UnrealNetwork.h"
#include "Components/SkeletalMeshComponent.h"

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

void UTDItemInstance::OnEquipped(AActor* InOwner) // ����
{
	UWorld* World = InOwner->GetWorld(); // �ν��Ͻ� Ŭ�����̱� ������ ������ ���带 �����;� �Ѵ�. �׳� GetWorld()�� �����ϸ� NULL�̴�.
	if (IsValid(World))
	{
		const UTDItemStaticData* ItemStaticData = GetTDItemStaticData();

		FTransform TransformTemp; 
		ItemActor = World->SpawnActorDeferred<ATDItemActor>(ItemStaticData->TDItemActorClass, TransformTemp, InOwner); // ItemActor ����. ������ų �� ���ʸ� InOwner�� �����Ͽ� ĳ���Ͱ� ���ʰ� �ǵ�����.

		ItemActor->InitItemActor(this);
		ItemActor->OnEquipped();
		ItemActor->FinishSpawning(TransformTemp);

		ATDCharacter* TDCharacter = Cast<ATDCharacter>(InOwner);
		USkeletalMeshComponent* SkeletalMesh = TDCharacter ? TDCharacter->GetMesh() : nullptr;
		if (IsValid(SkeletalMesh))
		{
			ItemActor->AttachToComponent(SkeletalMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, ItemStaticData->AttachmentSocket); // AttachmentSocket ��ġ�� ������ ���̱�
		}
	}
}

void UTDItemInstance::OnUnequipped(AActor* InOwner) // ���� ����
{
	UWorld* World = InOwner->GetWorld();
	if (IsValid(World))
	{
		ItemActor->Destroy();
		ItemActor = nullptr;
	}
}

void UTDItemInstance::OnDropped(AActor* InOwner) // �ٴڿ� ����߸���
{
	UWorld* World = InOwner->GetWorld();
	if (IsValid(World))
	{
		ItemActor->OnDropped();
	}
}

const UTDItemStaticData* UTDItemInstance::GetTDItemStaticData() const
{
	return UTDAbilitySystemBPLibrary::GetTDItemStaticData(TDItemStaticDataClass);
}

void UTDItemInstance::OnRep_Equipped()
{
	
}
