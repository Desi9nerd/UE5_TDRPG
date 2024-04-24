#include "GAS/GameplayAbility/TDGA.h"

FString UTDGA::GetDescription(int32 AbilityLevel)
{
	return FString::Printf(TEXT("<Default>%s, </><Level>%d</>"), L"Default Ability Name - 기본 텍스트. 정보를 기입하세요.", AbilityLevel);
}

FString UTDGA::GetNextAbilityLevelDescription(int32 AbilityLevel)
{
	return FString::Printf(TEXT("<Default>Next Level: </><Level>%d</> \n<Default> 에 도달하면 더 큰 데미지 피해를 입힐 수 있습니다. </>"), AbilityLevel);
}

FString UTDGA::GetLockedDescription(int32 AbilityLevel)
{
	return FString::Printf(TEXT("<Default> %d 레벨에 도달하면 스킬이 해금됩니다. </>"), AbilityLevel);
}
