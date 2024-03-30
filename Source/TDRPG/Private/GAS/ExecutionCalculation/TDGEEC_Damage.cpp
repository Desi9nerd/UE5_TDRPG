#include "GAS/ExecutionCalculation/TDGEEC_Damage.h"
#include "AbilitySystemComponent.h"
#include "GAS/GameplayEffectContext/TDAbilityTypes.h"
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
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage);

	TDDamageStatics()
	{
		// Ŭ���� �̸�, ��� �̸�, Source �Ǵ� Target, ������ ���� ����. �ڼ��� ���� F12�� Ȯ��
		DEFINE_ATTRIBUTE_CAPTUREDEF(UTDAttributeSet, Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UTDAttributeSet, BlockChance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UTDAttributeSet, ArmorPenetration, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UTDAttributeSet, CriticalHitChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UTDAttributeSet, CriticalHitResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UTDAttributeSet, CriticalHitDamage, Source, false);
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
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitDamageDef);
}

void UTDGEEC_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	// SourceASC�� TargetASC�� �ִ� ��� AvatorActor�� �����´�
	AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	// Source, Target ĳ������ ������ ����ϱ� ���� IICombat ������ �����
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
	float Damage = 0.f;
	for (const TTuple<FGameplayTag, FGameplayTag>& Pair : FTDGameplayTags::GetTDGameplayTags().DamageTypesToResistances)
	{
		const float DamageTypeValue = Spec.GetSetByCallerMagnitude(Pair.Key);
		Damage += DamageTypeValue;
	}

	//**************************************************************************//
	//* Block
	// Ÿ���� 'BlockChance�� ĸó' + 'Block ���� ����(=bBlocked)�� �Ǵ�'
	float TargetBlockChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef, EvaluationParameters, TargetBlockChance); // BlockChance ĸó
	TargetBlockChance = FMath::Max<float>(TargetBlockChance, 0.f);

	const bool bBlocked = FMath::RandRange(1, 100) < TargetBlockChance; // Block ���� ���� �Ǵ�

	// UTDAbilitySystemBPLibrary�� Block Hit ���� ���θ� �ѱ�
	FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();
	UTDAbilitySystemBPLibrary::SetIsBlockedHit(EffectContextHandle, bBlocked);

	//* ������ ���. Block ����: Damage ����, Block ����: Damage
	Damage = bBlocked ? Damage / 2.f : Damage;
	//**************************************************************************//


	//**************************************************************************//
	//* Armor, ArmorPenetration
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
	//**************************************************************************//


	//**************************************************************************//
	//* Critical HitChance, HitResistance, HitDamage
	float SourceCriticalHitChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitChanceDef, EvaluationParameters, SourceCriticalHitChance);
	SourceCriticalHitChance = FMath::Max<float>(SourceCriticalHitChance, 0.f);

	float TargetCriticalHitResistance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitResistanceDef, EvaluationParameters, TargetCriticalHitResistance);
	TargetCriticalHitResistance = FMath::Max<float>(TargetCriticalHitResistance, 0.f);

	float SourceCriticalHitDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitDamageDef, EvaluationParameters, SourceCriticalHitDamage);
	SourceCriticalHitDamage = FMath::Max<float>(SourceCriticalHitDamage, 0.f);

	const FRealCurve* CriticalHitResistanceCurve = TDDA_CharacterClass->DamageCalculationCoefficients->FindCurve(FName("CriticalHitResistance"), FString());
	const float CriticalHitResistanceCoefficient = CriticalHitResistanceCurve->Eval(TargetCombatInterface->GetPlayerLevel());

	// Critical Hit Resistance�� Critical Hit Ȯ���� ����
	const float EffectiveCriticalHitChance = SourceCriticalHitChance - TargetCriticalHitResistance * CriticalHitResistanceCoefficient;
	const bool bCriticalHit = FMath::RandRange(1, 100) < EffectiveCriticalHitChance; // Critical Hit ���� ����

	// UTDAbilitySystemBPLibrary�� Critical Hit ���θ� �ѱ�.
	UTDAbilitySystemBPLibrary::SetIsCriticalHit(EffectContextHandle, bCriticalHit);

	//** ������ ���. Critical Hit �� ������ 2��
	Damage = bCriticalHit ? 2.f * Damage + SourceCriticalHitDamage : Damage;
	//**************************************************************************//


	//* ���� ���� �������� �ѱ�
	const FGameplayModifierEvaluatedData EvaluatedData(UTDAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
