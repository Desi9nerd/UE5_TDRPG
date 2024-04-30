#include "Component/TDInventoryComponent.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Player/TDPlayerController.h"

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
	TDPlayerController->CreateInventoryCategoryWidgets();
}

void UTDInventoryComponent::InitializeInventory()
{
	if (TDPlayerController->IsLocalController())
	{
		Client_InitializeInventory();
	}
}

void UTDInventoryComponent::Client_InitializeInventory_Implementation()
{
	AmountOfSlots = 16;

	// AmountOfSlots 만큼 슬롯을 만들기위해 TArray 사이즈를 맞춰줌. 
	WeaponCategory.SetNum(AmountOfSlots);
	ArmorCategory.SetNum(AmountOfSlots);
	PotionCategory.SetNum(AmountOfSlots);
	FoodCategory.SetNum(AmountOfSlots);

	//FString Temp = FString::FromInt(WeaponCategory.Num());
	//UE_LOG(LogTemp, Warning, TEXT("WeaponCategory Size = %s"), *Temp);
}
