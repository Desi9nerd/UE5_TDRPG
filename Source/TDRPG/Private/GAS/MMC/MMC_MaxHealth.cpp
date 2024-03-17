#include "GAS/MMC/MMC_MaxHealth.h"
#include "GAS/TDAttributeSet.h"
#include "Interface/ICombat.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{
	VigorCaptureDefinition.AttributeToCapture = UTDAttributeSet::GetVigorAttribute();
	VigorCaptureDefinition.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	VigorCaptureDefinition.bSnapshot = false; // snapshot는 EffectSpec이 생성된 시점에 attribute를 캡처한다. 

	RelevantAttributesToCapture.Add(VigorCaptureDefinition);
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// Source와 Target의 Tag들을 모은다
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Vigor = 0.f;
	GetCapturedAttributeMagnitude(VigorCaptureDefinition, Spec, EvaluationParameters, Vigor);
	Vigor = FMath::Max<float>(Vigor, 0.f);

	IICombat* CombatInterface = Cast<IICombat>(Spec.GetContext().GetSourceObject());
	const int32 PlayerLevel = CombatInterface->GetPlayerLevel();

	return 80.f + 2.5f * Vigor + 10.f * PlayerLevel;
}
