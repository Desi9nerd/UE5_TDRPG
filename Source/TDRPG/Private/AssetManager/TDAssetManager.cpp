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

	// InitGlobeData() ���� InitTargetDataScriptStructCache()�� ���� TargetDataStrcutCache�� Initialize����
	// TDAT_TargetData���� TargetData�� ����ϱ����� �ʿ�.
	UAbilitySystemGlobals::Get().InitGlobalData(); 
}
