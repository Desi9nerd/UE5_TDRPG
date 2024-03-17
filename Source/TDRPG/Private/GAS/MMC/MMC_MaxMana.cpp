#include "GAS/MMC/MMC_MaxMana.h"
#include "GAS/TDAttributeSet.h"
#include "Interface/ICombat.h"

UMMC_MaxMana::UMMC_MaxMana()
{
	IntCaptureDefinition.AttributeToCapture = UTDAttributeSet::GetIntelligenceAttribute();
	IntCaptureDefinition.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	IntCaptureDefinition.bSnapshot = false;

	RelevantAttributesToCapture.Add(IntCaptureDefinition);
}

float UMMC_MaxMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// Source와 Target의 Tag들을 모은다
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Int = 0.f;
	GetCapturedAttributeMagnitude(IntCaptureDefinition, Spec, EvaluationParameters, Int);
	Int = FMath::Max<float>(Int, 0.f);

	IICombat* CombatInterface = Cast<IICombat>(Spec.GetContext().GetSourceObject());
	const int32 PlayerLevel = CombatInterface->GetPlayerLevel();

	return 50.f + 2.5f * Int + 15.f * PlayerLevel;
}
