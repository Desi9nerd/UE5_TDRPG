#include "GAS/ExecutionCalculation/TDGEEC_Damage.h"
#include "AbilitySystemComponent.h"
#include "GAS/TDAttributeSet.h"

struct TDDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor); // FGameplayEffectAttributeDefinition ArmorDef를 생성하는 매크로

	TDDamageStatics()
	{
		// 클래스 이름, 멤버 이름, Source 또는 Target, 스냅샷 할지 여부. 자세한 사항 F12로 확인
		DEFINE_ATTRIBUTE_CAPTUREDEF(UTDAttributeSet, Armor, Target, false);
	}
};

static const TDDamageStatics& DamageStatics()
{
	// static 변수 생성 후 변수 리턴
	static TDDamageStatics DStatics;
	return DStatics;
}

UTDGEEC_Damage::UTDGEEC_Damage()
{
	// Armor attribute를 캡처하여 추후에 계산에 이용
	// TArray<FGameplayEffectAttributeCaptureDefinition> RelevantAttributesToCapture; 배열에 담음
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
}

void UTDGEEC_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	// SourceASC와 TargetASC가 있는 경우 AvatorActor를 가져온다
	const AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	const AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	// Source와 Target의 Tag들을 모은다.
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Armor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, EvaluationParameters, Armor);
	Armor = FMath::Max<float>(0.f, Armor);
	++Armor;

	const FGameplayModifierEvaluatedData EvaluatedData(DamageStatics().ArmorProperty, EGameplayModOp::Additive, Armor);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
