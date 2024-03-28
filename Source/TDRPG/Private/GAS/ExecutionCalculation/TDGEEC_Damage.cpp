#include "GAS/ExecutionCalculation/TDGEEC_Damage.h"
#include "AbilitySystemComponent.h"
#include "GAS/TDAttributeSet.h"
#include "GameplayTags/TDGameplayTags.h"
#include "GAS/TDAbilitySystemBPLibrary.h"
#include "GAS/Data/TDDA_CharacterClass.h"
#include "Interface/ICombat.h"

struct TDDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor); // FGameplayEffectAttributeDefinition ArmorDef�� �����ϴ� ��ũ��
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);

	TDDamageStatics()
	{
		// Ŭ���� �̸�, ��� �̸�, Source �Ǵ� Target, ������ ���� ����. �ڼ��� ���� F12�� Ȯ��
		DEFINE_ATTRIBUTE_CAPTUREDEF(UTDAttributeSet, Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UTDAttributeSet, BlockChance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UTDAttributeSet, ArmorPenetration, Source, false);
	}
};

static const TDDamageStatics& DamageStatics()
{
	// static ���� ���� �� ���� ����
	static TDDamageStatics DStatics;
	return DStatics;
}

UTDGEEC_Damage::UTDGEEC_Damage()
{
	// Armor attribute�� ĸó�Ͽ� ���Ŀ� ��꿡 �̿�
	// TArray<FGameplayEffectAttributeCaptureDefinition> RelevantAttributesToCapture; �迭�� ����
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().BlockChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArmorPenetrationDef);
}

void UTDGEEC_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	// SourceASC�� TargetASC�� �ִ� ��� AvatorActor�� �����´�
	AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	// Source(������), Target(�ǰ���) ĳ������ ������ ����ϱ� ���� IICombat ������ �����
	IICombat* SourceCombatInterface = Cast<IICombat>(SourceAvatar);
	IICombat* TargetCombatInterface = Cast<IICombat>(TargetAvatar);

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	// Source�� Target�� Tag���� ������.
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;


	// ������ ������ ���. GetSetByCallerMagnitude�� SetByCaller modifier�� magnitude���� ������
	float Damage = Spec.GetSetByCallerMagnitude(FTDGameplayTags::GetTDGameplayTags().Damage);

	// Ÿ���� 'BlockChance�� ĸó' + 'Block ���� ����(=bBlocked)�� �Ǵ�'
	float TargetBlockChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef, EvaluationParameters, TargetBlockChance); // BlockChance ĸó
	TargetBlockChance = FMath::Max<float>(TargetBlockChance, 0.f);

	const bool bBlocked = FMath::RandRange(1, 100) < TargetBlockChance; // Block ���� ���� �Ǵ�

	//* ������ ���. Block ����: Damage ����, Block ����: Damage
	Damage = bBlocked ? Damage / 2.f : Damage;

	float TargetArmor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, EvaluationParameters, TargetArmor); // TargetArmor ĸó
	TargetArmor = FMath::Max<float>(TargetArmor, 0.f); 

	float SourceArmorPenetration = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorPenetrationDef, EvaluationParameters, SourceArmorPenetration); // SourceArmorPenetration ĸó
	SourceArmorPenetration = FMath::Max<float>(SourceArmorPenetration, 0.f);

	const UTDDA_CharacterClass* TDDA_CharacterClass = UTDAbilitySystemBPLibrary::GetTDDA_CharacterClass(SourceAvatar);
	check(TDDA_CharacterClass->DamageCalculationCoefficients);
	const FRealCurve* ArmorPenetrationCurve = TDDA_CharacterClass->DamageCalculationCoefficients->FindCurve(FName("ArmorPenetration"), FString()); // CT_DamageCalculationCoeffients�� "ArmorPenetration" ������ 
	const float ArmorPenetrationCoefficient = ArmorPenetrationCurve->Eval(SourceCombatInterface->GetPlayerLevel()); // ������ �´� Column�� �����Ͽ� ���� ������

	// Target's Armor�� Source's ArmorPenetration��ŭ ����.
	TargetArmor *= (100 - SourceArmorPenetration * ArmorPenetrationCoefficient) / 100.f;
	const float EffectiveArmor = TargetArmor;

	const FRealCurve* EffectiveArmorCurve = TDDA_CharacterClass->DamageCalculationCoefficients->FindCurve(FName("EffectiveArmor"), FString());
	const float EffectiveArmorCoefficient = EffectiveArmorCurve->Eval(TargetCombatInterface->GetPlayerLevel());

	//* ������ ���. EffectiveArmor �ۼ�Ʈ����ŭ ������ ����
	Damage *= (100 - EffectiveArmor * EffectiveArmorCoefficient) / 100.f;

	//* ���� ���� �������� �ѱ�
	const FGameplayModifierEvaluatedData EvaluatedData(UTDAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
