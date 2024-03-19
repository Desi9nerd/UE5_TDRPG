#include "GAS/Data/TDDataAttribute.h"

FTDDataAttributeInfo UTDDataAttribute::FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFound) const
{
	for (const FTDDataAttributeInfo& Info : AttributeInfo)
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

	return FTDDataAttributeInfo();
}
