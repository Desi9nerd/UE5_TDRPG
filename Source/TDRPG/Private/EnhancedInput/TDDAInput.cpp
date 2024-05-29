#include "EnhancedInput/TDDAInput.h"

const UInputAction* UTDDAInput::FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	for (const FTDInputAction& Action : AbilityInputActions)
	{
		if (Action.InputAction && Action.InputTag == InputTag)
		{
			return Action.InputAction;
		}
	}

	if (bLogNotFound)
	{
		//UE_LOG(LogTemp, Error, TEXT("AbilityInputAction�� ã�� �� �����ϴ�. InputTag [%s], on InputConfig [%s]"), *InputTag.ToString(), *GetNameSafe(this));
	}

	return nullptr;
}
