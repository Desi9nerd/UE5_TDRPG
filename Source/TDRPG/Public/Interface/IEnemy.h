#pragma once
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IEnemy.generated.h"

UINTERFACE(MinimalAPI)
class UIEnemy : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TDRPG_API IIEnemy
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void HighlightActor() = 0; // Enemy를 가리키면 외곽선
	virtual void UnHighlightActor() = 0;
};
