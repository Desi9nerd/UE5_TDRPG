#include "GAS/Data/TDDA_Attribute.h"

FDA_Attribute UTDDA_Attribute::FindDA_AttributeForTag(const FGameplayTag& AttributeTag, bool bLogNotFound) const
{
	for (const FDA_Attribute& Info : DA_AttributeInfo)
	{
		if (Info.AttributeTag.MatchesTagExact(AttributeTag)) // AttributeInfo배열에서 AttributeTag 찾기
		{
			return Info;
		}
	}

	// AttributeInfo 배열에서 AttributeTag를 못 찾으면 아래의 메시지를 띄운다
	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find Info for AttributeTag [%s] on AttributeInfo [%s]."), *AttributeTag.ToString(), *GetNameSafe(this));
	}

	return FDA_Attribute();
}
