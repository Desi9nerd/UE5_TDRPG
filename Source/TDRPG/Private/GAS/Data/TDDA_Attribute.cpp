#include "GAS/Data/TDDA_Attribute.h"

FDA_Attribute UTDDA_Attribute::FindDA_AttributeForTag(const FGameplayTag& AttributeTag, bool bLogNotFound) const
{
	for (const FDA_Attribute& Info : DA_AttributeInfo)
	{
		if (Info.AttributeTag.MatchesTagExact(AttributeTag)) // AttributeInfo�迭���� AttributeTag ã��
		{
			return Info;
		}
	}

	// AttributeInfo �迭���� AttributeTag�� �� ã���� �Ʒ��� �޽����� ����
	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find Info for AttributeTag [%s] on AttributeInfo [%s]."), *AttributeTag.ToString(), *GetNameSafe(this));
	}

	return FDA_Attribute();
}
