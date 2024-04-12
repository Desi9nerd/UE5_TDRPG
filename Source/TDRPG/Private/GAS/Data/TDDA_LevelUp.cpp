#include "GAS/Data/TDDA_LevelUp.h"

int32 UTDDA_LevelUp::FindDA_LevelUpForExp(int32 Exp)
{
	int32 Lv = 1; // 캐릭터 레벨
	bool bSearching = true;

	// 더 이상 레벨업이 필요 없을때까지 while문을 돌린다.
	while (bSearching)
	{
		// DA_LevelUpInfo[1] = Lv 1 Info
		// DA_LevelUpInfo[2] = Lv 1 Info
		// 아래 조건을 만족하면 레벨업을 할 수 없는 상황.
		if (DA_LevelUpInfo.Num() - 1 <= Lv) return Lv;

		// 경험치(Exp)가 해당 레벨의 레벨업 필요조건 보다 크다면 레벨업 시킨다. 
		if (Exp >= DA_LevelUpInfo[Lv].LevelUpRequirement)
		{
			++Lv; // 레벨 업
		}
		else
		{
			bSearching = false;
		}
	}

	return Lv;
}