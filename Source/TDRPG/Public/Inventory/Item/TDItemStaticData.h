#pragma once
#include "CoreMinimal.h"
#include "TDItemStaticData.generated.h"

class ATDItemActor;

/**
 * 
 */

UENUM(BlueprintType)
enum class EItemState : uint8
{
	None	 UMETA(DisplayName = "None"),
	Stored	 UMETA(DisplayName = "Stored"),
	Equipped UMETA(DisplayName = "Equipped"),
	Dropped  UMETA(DisplayName = "Dropped"),
};

UCLASS(BlueprintType, Blueprintable)
class UTDItemStaticData : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<ATDItemActor> TDItemActorClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName AttachmentSocket; // �ش� �������� ĳ���Ϳ� ������ �� ���Ǵ� ���� �̸�
};
