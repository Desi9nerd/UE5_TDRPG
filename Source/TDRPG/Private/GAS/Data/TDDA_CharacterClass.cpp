#include "GAS/Data/TDDA_CharacterClass.h"

FCharacterClassDefaultInfo UTDDA_CharacterClass::GetClassDefaultInfo(ECharacterClass CharacterClass)
{
	return CharacterClassInformation.FindChecked(CharacterClass);
}
