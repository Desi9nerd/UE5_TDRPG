#pragma once
#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "TDDAInput.h"
#include "TDEnhancedInputComponent.generated.h"

/**
 * 
 */
UCLASS()
class TDRPG_API UTDEnhancedInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
	void BindAbilityActions(const UTDDAInput* InputData, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc);
};

template <class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
void UTDEnhancedInputComponent::BindAbilityActions(const UTDDAInput* InputData, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc)
{
	check(InputData);

	// �����Ϳ��� �Ҵ��� AbilityInputActions�� Bind
	for (const FTDInputAction& Action : InputData->AbilityInputActions)
	{
		if (Action.InputAction && Action.InputTag.IsValid())
		{
			if (PressedFunc) // �Է��� ��������
			{
				BindAction(Action.InputAction, ETriggerEvent::Started, Object, PressedFunc, Action.InputTag);
			}
			if (ReleasedFunc) // �Է��� ������ ������
			{
				BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputTag);
			}
			if (HeldFunc) // �Է��� ��� ������ ������
			{
				BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, HeldFunc, Action.InputTag);
			}
		}
	}
}
