#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "TDPlayerController.generated.h"

class ATDHUD;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class IIEnemy;
class UTDDAInput;
class UTDAbilitySystemComponent;
class USplineComponent;
class UTDWidgetComponent;
class UTDUW_Inventory;
class UTDUW_InventoryCategory;

/**
 * 
 */

UCLASS()
class TDRPG_API ATDPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATDPlayerController();
	virtual void PlayerTick(float DeltaTime) override;
	ATDHUD* GetTDHUD();

	UFUNCTION(Client, Reliable)
	void ShowDamageNumber(float DamageAmount, ACharacter* TargetCharacter, bool bBlockedHit, bool bCriticalHit); 

	void CreateInventoryCategoryWidgets();
	UFUNCTION(Client, Reliable) // Client RPC
	void Client_CreateInventoryCategoryWidgets();


protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	void Move(const FInputActionValue& InputActionValue);
	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);
	TObjectPtr<UTDAbilitySystemComponent> GetASC();
	void AutoRun();
	void ShiftPressed() { bShiftKeyDown = true; };
	void ShiftReleased() { bShiftKeyDown = false; };

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> RPGContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> ShiftAction;
	bool bShiftKeyDown = false;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UTDDAInput> InputData;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UTDAbilitySystemComponent> TDASC;

	UPROPERTY()
	TObjectPtr<APawn> PlayerControlledPawn;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UTDWidgetComponent> DamageTextComponentClass; // 데미지 위젯

	//** 이동
	UPROPERTY(EditDefaultsOnly)
	float AutoRunAcceptanceRadius = 50.f;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline; // 이동경로 Spline

	FVector CachedDestination = FVector::ZeroVector;
	float FollowTime = 0.f;
	float ShortPressThreshold = 0.5f;
	bool bAutoRunning = false;
	bool bTargeting = false;
	
	void CursorTrace(); // 마우스 CursorTrace
	FHitResult CursorHit; // 마우스 CursorHit
	IIEnemy* LastActor;
	IIEnemy* ThisActor;




	////////////////////////////////////////////////////////////
	//void OnDropItemTriggered(const FInputActionValue& Value);
	//void OnEquipNextTriggered(const FInputActionValue& Value);
	//void OnUnequipTriggered(const FInputActionValue& Value);
	//UPROPERTY(EditDefaultsOnly)
	//class UInputAction* DropItemInputAction;
	//UPROPERTY(EditDefaultsOnly)
	//class UInputAction* EquipNextInputAction;
	//UPROPERTY(EditDefaultsOnly)
	//class UInputAction* UnequipInputAction;

	void OnOpenInventoryPressed(const FInputActionValue& InputActionValue);

	void InitializeWidget();
	UFUNCTION(Client, Reliable) // Client RPC
	void Client_InitializeWidget();

	void OpenCloseInventoryWidget(bool bOpen);
	UFUNCTION(Client, Reliable) // Client RPC
	void Client_OpenCloseInventoryWidget(bool bOpen);

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> OpenInventoryInputAction;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> MainWidgetClass; 
	UPROPERTY()
	TObjectPtr<UUserWidget> TDMainWidget;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UTDUW_Inventory> InventoryWidgetClass; 
	UPROPERTY()
	TObjectPtr<UTDUW_Inventory> TDInventoryWidget;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UTDUW_InventoryCategory> InventoryCategoryWidgetClass;
	UPROPERTY()
	TObjectPtr<UTDUW_InventoryCategory> InventoryCategoryWidget;
	


	bool bInventoryIsOpen;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UDataTable> InventoryCategoryDataTable;

	UPROPERTY()
	TObjectPtr<ATDHUD> TDHUD;
};
