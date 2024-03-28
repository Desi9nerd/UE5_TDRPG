#include "GAS/ExecutionCalculation/TDGEEC_Damage.h"
#include "AbilitySystemComponent.h"
#include "GAS/TDAttributeSet.h"
#include "GameplayTags/TDGameplayTags.h"
#include "GAS/TDAbilitySystemBPLibrary.h"
#include "GAS/Data/TDDA_CharacterClass.h"
#include "Interface/ICombat.h"

struct TDDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor); // FGameplayEffectAttributeDefinition ArmorDef를 생성하는 매크로
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);

	TDDamageStatics()
	{
		// 클래스 이름, 멤버 이름, Source 또는 Target, 스냅샷 할지 여부. 자세한 사항 F12로 확인
		DEFINE_ATTRIBUTE_CAPTUREDEF(UTDAttributeSet, Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UTDAttributeSet, BlockChance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UTDAttributeSet, ArmorPenetration, Source, false);
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
	RelevantAttributesToCapture.Add(DamageStatics().BlockChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArmorPenetrationDef);
}

void UTDGEEC_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	// SourceASC와 TargetASC가 있는 경우 AvatorActor를 가져온다
	AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	// Source(공격자), Target(피격자) 캐릭터의 레벨을 사용하기 위해 IICombat 변수를 만든다
	IICombat* SourceCombatInterface = Cast<IICombat>(SourceAvatar);
	IICombat* TargetCombatInterface = Cast<IICombat>(TargetAvatar);

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	// Source와 Target의 Tag들을 모은다.
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;


	// 데미지 변수에 담기. GetSetByCallerMagnitude는 SetByCaller modifier의 magnitude값을 가져옴
	float Damage = Spec.GetSetByCallerMagnitude(FTDGameplayTags::GetTDGameplayTags().Damage);

	// 타겟의 'BlockChance를 캡처' + 'Block 성공 여부(=bBlocked)를 판단'
	float TargetBlockChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef, EvaluationParameters, TargetBlockChance); // BlockChance 캡처
	TargetBlockChance = FMath::Max<float>(TargetBlockChance, 0.f);

	const bool bBlocked = FMath::RandRange(1, 100) < TargetBlockChance; // Block 성공 여부 판단

	//* 데미지 계산. Block 성공: Damage 절반, Block 실패: Damage
	Damage = bBlocked ? Damage / 2.f : Damage;

	float TargetArmor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, EvaluationParameters, TargetArmor); // TargetArmor 캡처
	TargetArmor = FMath::Max<float>(TargetArmor, 0.f); 

	float SourceArmorPenetration = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorPenetrationDef, EvaluationParameters, SourceArmorPenetration); // SourceArmorPenetration 캡처
	SourceArmorPenetration = FMath::Max<float>(SourceArmorPenetration, 0.f);

	const UTDDA_CharacterClass* TDDA_CharacterClass = UTDAbilitySystemBPLibrary::GetTDDA_CharacterClass(SourceAvatar);
	check(TDDA_CharacterClass->DamageCalculationCoefficients);
	const FRealCurve* ArmorPenetrationCurve = TDDA_CharacterClass->DamageCalculationCoefficients->FindCurve(FName("ArmorPenetration"), FString()); // CT_DamageCalculationCoeffients의 "ArmorPenetration" 데이터 
	const float ArmorPenetrationCoefficient = ArmorPenetrationCurve->Eval(SourceCombatInterface->GetPlayerLevel()); // 레벨에 맞는 Column에 접근하여 값을 가져옴

	// Target's Armor를 Source's ArmorPenetration만큼 차감.
	TargetArmor *= (100 - SourceArmorPenetration * ArmorPenetrationCoefficient) / 100.f;
	const float EffectiveArmor = TargetArmor;

	const FRealCurve* EffectiveArmorCurve = TDDA_CharacterClass->DamageCalculationCoefficients->FindCurve(FName("EffectiveArmor"), FString());
	const float EffectiveArmorCoefficient = EffectiveArmorCurve->Eval(TargetCombatInterface->GetPlayerLevel());

	//* 데미지 계산. EffectiveArmor 퍼센트지만큼 데미지 차감
	Damage *= (100 - EffectiveArmor * EffectiveArmorCoefficient) / 100.f;

	//* 최종 계산된 데미지를 넘김
	const FGameplayModifierEvaluatedData EvaluatedData(UTDAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
