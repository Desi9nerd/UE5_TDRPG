#include "GAS/Data/TDDA_LevelUp.h"

int32 UTDDA_LevelUp::FindDA_LevelUpForExp(int32 Exp)
{
	int32 Lv = 1; // ĳ���� ����
	bool bSearching = true;

	// �� �̻� �������� �ʿ� ���������� while���� ������.
	while (bSearching)
	{
		// DA_LevelUpInfo[1] = Lv 1 Info
		// DA_LevelUpInfo[2] = Lv 1 Info
		// �Ʒ� ������ �����ϸ� �������� �� �� ���� ��Ȳ.
		if (DA_LevelUpInfo.Num() - 1 <= Lv) return Lv;

		// ����ġ(Exp)�� �ش� ������ ������ �ʿ����� ���� ũ�ٸ� ������ ��Ų��. 
		if (Exp >= DA_LevelUpInfo[Lv].LevelUpRequirement)
		{
			++Lv; // ���� ��
		}
		else
		{
			bSearching = false;
		}
	}

	return Lv;
}