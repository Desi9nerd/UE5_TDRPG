﻿#pragma once
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "TDPlayerController.generated.h"

struct FInputActionValue;
class ATDItemActor;
class ATDDecalActor;
class ATDHUD;
class UInputMappingContext;
class UInputAction;
class UNiagaraSystem;
class ATDCharacter;
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
	TObjectPtr<ATDCharacter> GetTDCharacter();

	UFUNCTION(Client, Reliable)
	void ShowDamageNumber(float DamageAmount, ACharacter* TargetCharacter, bool bBlockedHit, bool bCriticalHit); 

	void ShowDecal();
	void HideDecal();

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
	void UpdateDecalLocation();

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
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UNiagaraSystem> ClickMoveEffect; // 이동 클릭 시 보이는 이펙트

	FVector CachedDestination = FVector::ZeroVector;
	float FollowTime = 0.f;
	float ShortPressThreshold = 0.5f;
	bool bAutoRunning = false;
	bool bTargeting = false;
	
	void CursorTrace(); // 마우스 CursorTrace
	FHitResult CursorHit; // 마우스 CursorHit
	IIEnemy* LastActor;
	IIEnemy* ThisActor;

	//** Decal
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ATDDecalActor> DecalClass;
	UPROPERTY()
	TObjectPtr<ATDDecalActor> Decal;


	////////////////////////////////////////////////////////////

	void OnOpenInventoryTriggered(const FInputActionValue& Value);
	void OnPickupItemTriggered(const FInputActionValue& Value);
	
	void OpenCloseInventoryWidget(bool bOpen);
	UFUNCTION(Client, Reliable) // Client RPC
	void Client_OpenCloseInventoryWidget(bool bOpen);

	void OnMouseWheel(const FInputActionValue& Value);
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> ZoomInputAction;
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> OpenInventoryInputAction;
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> PickUpItemInputAction;
	
	//UPROPERTY(EditDefaultsOnly)
	//TSubclassOf<UTDUW_Inventory> InventoryWidgetClass; 
	//UPROPERTY()
	//TObjectPtr<UTDUW_Inventory> TDInventoryWidget;

	bool bInventoryIsOpen;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UDataTable> InventoryCategoryDataTable;

	UPROPERTY()
	TObjectPtr<ATDHUD> TDHUD;

	UPROPERTY()
	TObjectPtr<ATDCharacter> TDCharacter;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> OverlappingItem;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USoundBase> Sound_OpenInventory;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USoundBase> Sound_CloseInventory;

	bool bTemp = false;
};
