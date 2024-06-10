#include "SaveGame/TDSaveGame_Load.h"

FSavedMap UTDSaveGame_Load::GetSavedMapWithMapName(const FString& InMapName)
{
	for (const FSavedMap& Map : SavedMaps)
	{
		if (Map.MapAssetName == InMapName) // 해당 맵 이름을 찾으면
		{
			return Map; // 해당 맵의 FSaveMap 데이터 반환.
		}
	}

	return FSavedMap();
}

// 매개변수로 들어온 이름의 맵이 있는지 확인하는 함수.
bool UTDSaveGame_Load::HasMap(const FString& InMapName)
{
	for (const FSavedMap& Map : SavedMaps)
	{
		if (Map.MapAssetName == InMapName)
		{
			return true;
		}
	}

	return false;
}
