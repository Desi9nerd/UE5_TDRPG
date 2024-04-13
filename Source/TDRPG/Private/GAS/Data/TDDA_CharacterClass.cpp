#include "GAS/Data/TDDA_CharacterClass.h"

FDA_CharacterClass UTDDA_CharacterClass::GetDA_ClassClass(ECharacterClass CharacterClass)
{
	return CharacterClassTMap.FindChecked(CharacterClass);
}
