#pragma once
#include "CoreMinimal.h"
#include "Character/TDBaseCharacter.h"
#include "Interface/IEnemy.h"
#include "Library/TDItemLibrary.h"
#include "UI/WidgetController/TDWidgetControllerOverlay.h"
#include "TDEnemyCharacter.generated.h"

class UWidgetComponent;
class UBehaviorTree;
class ATDAIController;

/**
 * 
 */
UCLASS()
class TDRPG_API ATDEnemyCharacter : public ATDBaseCharacter, public IIEnemy
{
	GENERATED_BODY()

public:
	ATDEnemyCharacter();
	virtual void PossessedBy(AController* NewController) override;
	FORCEINLINE void SetEnemyLevel(int32 InEnemyLevel) { Level = InEnemyLevel; }

//=== IEnemy ==================================================================
public:
	virtual void HighlightActor() override;	  // ¿Ü°û¼± È¿°ú On
	virtual void UnHighlightActor() override; // ¿Ü°û¼± È¿°ú Off
	virtual void SetCombatTarget_Implementation(AActor* InCombatTarget) override;
	virtual AActor* GetCombatTarget_Implementation() const override;
	virtual AActor* GetCombatTargetCPP() const override;
//=== IEnemy ==================================================================
	
//=== ICombat =================================================================
public:
	virtual int32 GetPlayerLevel() override;
	virtual void Die(const FVector& RagdollImpulse) override;
//=== ICombat =================================================================

	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bHitReacting = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float LifeSpanUntilDead = 5.f;

	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	TObjectPtr<AActor> CombatTarget;

protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
	virtual void InitializeDefaultAttributes() const override;
	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	int32 Level = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;

	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;
	UPROPERTY()
	TObjectPtr<ATDAIController> TDAIController;

	UFUNCTION()
	void SpawnItemsAfterDeath();

private:
	UFUNCTION()
	void SpawnLootItem();

	TArray<FRotator> EvenlySpacedRotators(const FVector& ForwardVector, const FVector& Axis, float Spread, int32 NumOfRotators);

	UPROPERTY(EditDefaultsOnly, Category = "Drop Item", Meta = (AllowPrivateAccess = true))
	float MinSpawnDistance = 25.f;
	UPROPERTY(EditDefaultsOnly, Category = "Drop Item", Meta = (AllowPrivateAccess = true))
	float MaxSpawnDistance = 150.f;

	TArray<FLootItem> LootItems;

	TArray<FRotator> LootRotations;
	int32 SpawnLoopCnt;

	FTimerHandle LootTimer;

};
