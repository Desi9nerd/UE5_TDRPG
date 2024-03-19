#include "AssetManager/TDAssetManager.h"
#include "GameplayTags/TDGameplayTags.h"

UTDAssetManager& UTDAssetManager::GetTDAssetManager()
{
	check(GEngine);

	UTDAssetManager* TDAssetManager = Cast<UTDAssetManager>(GEngine->AssetManager);
	return *TDAssetManager;
}

void UTDAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FTDGameplayTags::InitializeNativeGameplayTags();
}
