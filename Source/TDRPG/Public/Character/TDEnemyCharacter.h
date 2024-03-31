#pragma once
#include "CoreMinimal.h"
#include "Character/TDBaseCharacter.h"
#include "Interface/IEnemy.h"
#include "GAS/Data/TDDA_CharacterClass.h"
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

	//** IEnemy 함수
	virtual void HighlightActor() override;	  // 외곽선 효과 On
	virtual void UnHighlightActor() override; // 외곽선 효과 Off
	virtual void SetCombatTarget_Implementation(AActor* InCombatTarget) override;
	virtual AActor* GetCombatTarget_Implementation() const override;

	//** ICombat 함수
	virtual int32 GetPlayerLevel() override;
	virtual void Die() override;

	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bHitReacting = false;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	float BaseWalkSpeed = 600.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float LifeSpanUntilDead = 5.f;

	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	TObjectPtr<AActor> CombatTarget;

protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
	virtual void InitializeDefaultAttributes() const override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	int32 Level = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;

	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;
	UPROPERTY()
	TObjectPtr<ATDAIController> TDAIController;

};
