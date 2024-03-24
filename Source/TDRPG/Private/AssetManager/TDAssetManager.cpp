#include "AssetManager/TDAssetManager.h"
#include "GameplayTags/TDGameplayTags.h"
#include "AbilitySystemGlobals.h"

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

	// InitGlobeData() 내의 InitTargetDataScriptStructCache()를 통해 TargetDataStrcutCache를 Initialize해줌
	// TDAT_TargetData에서 TargetData를 사용하기위해 필요.
	UAbilitySystemGlobals::Get().InitGlobalData(); 
}
