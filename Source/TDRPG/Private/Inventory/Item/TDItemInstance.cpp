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

void UTDItemInstance::OnEquipped(AActor* InOwner) // 장착
{
	UWorld* World = InOwner->GetWorld(); // 인스턴스 클래스이기 때문에 오너의 월드를 가져와야 한다. 그냥 GetWorld()를 선언하면 NULL이다.
	if (IsValid(World))
	{
		const UTDItemStaticData* ItemStaticData = GetTDItemStaticData();

		FTransform TransformTemp; 
		ItemActor = World->SpawnActorDeferred<ATDItemActor>(ItemStaticData->TDItemActorClass, TransformTemp, InOwner); // ItemActor 스폰. 스폰시킬 때 오너를 InOwner로 지정하여 캐릭터가 오너가 되도록함.

		ItemActor->InitItemActor(this);
		ItemActor->OnEquipped();
		ItemActor->FinishSpawning(TransformTemp);

		ATDCharacter* TDCharacter = Cast<ATDCharacter>(InOwner);
		USkeletalMeshComponent* SkeletalMesh = TDCharacter ? TDCharacter->GetMesh() : nullptr;
		if (IsValid(SkeletalMesh))
		{
			ItemActor->AttachToComponent(SkeletalMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, ItemStaticData->AttachmentSocket); // AttachmentSocket 위치에 아이템 붙이기
		}
	}
}

void UTDItemInstance::OnUnequipped(AActor* InOwner) // 장착 해제
{
	UWorld* World = InOwner->GetWorld();
	if (IsValid(World))
	{
		ItemActor->Destroy();
		ItemActor = nullptr;
	}
}

void UTDItemInstance::OnDropped(AActor* InOwner) // 바닥에 떨어뜨리기
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
