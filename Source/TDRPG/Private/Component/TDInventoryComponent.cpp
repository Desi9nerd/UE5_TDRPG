#include "Component/TDInventoryComponent.h"

#include "Actor/TDItemActor.h"
#include "Character/TDCharacter.h"
#include "Components/GridPanel.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Player/TDPlayerController.h"
#include "UI/Widget/Inventory/TDUW_Inventory.h"

UTDInventoryComponent::UTDInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UTDInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	TDPlayerController = Cast<ATDPlayerController>(GetWorld()->GetFirstPlayerController());
	check(TDPlayerController);

	InitializeInventory();
	//TDPlayerController->CreateInventoryCategoryWidgets();
}

void UTDInventoryComponent::InitializeInventory()
{
	if (TDPlayerController->IsLocalController())
	{
		Client_InitializeInventory();
	}
}

void UTDInventoryComponent::SetSelectedInventoryCategory(const EItemCategory& InSelectedInventoryCategory)
{
	if (TDPlayerController->IsLocalController())
	{
		Client_SetSelectedInventoryCategory(InSelectedInventoryCategory);
	}
}

void UTDInventoryComponent::PickupItem()
{
	Server_PickupItem();
}

void UTDInventoryComponent::Server_PickupItem_Implementation()
{
	ATDCharacter* TDCharacter = Cast<ATDCharacter>(TDPlayerController->GetCharacter());
	TArray<AActor*> OverlappingActors;
	TDCharacter->SphereCollision->GetOverlappingActors(OverlappingActors, ATDItemActor::StaticClass());

	if (OverlappingActors.Num() > 0)
	{
		UE_LOG(LogTemp, Log, TEXT("Overlapping Actor!"));
	}
}

void UTDInventoryComponent::Client_SetSelectedInventoryCategory_Implementation(const EItemCategory& InSelectedInventoryCategory)
{
	SelectedInventoryCategory = InSelectedInventoryCategory;
}

void UTDInventoryComponent::Client_InitializeInventory_Implementation()
{
	// AmountOfSlots 만큼 슬롯을 만들기위해 TArray 사이즈를 맞춰줌. 
	WeaponCategory.SetNum(AmountOfSlots);
	ArmorCategory.SetNum(AmountOfSlots);
	PotionCategory.SetNum(AmountOfSlots);
	FoodCategory.SetNum(AmountOfSlots);

	//FString Temp = FString::FromInt(WeaponCategory.Num());
	//UE_LOG(LogTemp, Warning, TEXT("WeaponCategory Size = %s"), *Temp);
}

void UTDInventoryComponent::AddtoInventory(AActor* InItem)
{
	Client_AddtoInventory(InItem);
}

void UTDInventoryComponent::Client_AddtoInventory_Implementation(AActor* InItem)
{

}
