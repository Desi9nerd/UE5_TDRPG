#include "GAS/Data/TDDA_Ability.h"

FDA_Ability UTDDA_Ability::FindDA_AbilityForTag(const FGameplayTag& AbilityTag, bool bLogNotFound) const
{
	for (const FDA_Ability& Info : DA_AbilityInfo)
	{
		if (Info.AbilityTag == AbilityTag)
		{
			return Info;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find info for AbilityTag [%s] on AbilityInfo [%s]"), *AbilityTag.ToString(), *GetNameSafe(this));
	}

	return FDA_Ability();
}
