#include "GAS/GameplayAbility/TDGA.h"
#include "GAS/TDAttributeSet.h"
#include "GAS/AbilityTask/TDAT_TargetData.h"

void UTDGA::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	TargetDataAbilityTask = UTDAT_TargetData::CreateTargetDataUnderMouse(this);
	TargetDataAbilityTask->ReadyForActivation();
}

FString UTDGA::GetDescription(int32 AbilityLevel)
{
	return FString::Printf(TEXT("<Default>%s, </><Level>%d</>"), L"Default Ability Name - 기본 텍스트. 정보를 기입하세요.", AbilityLevel);
}

FString UTDGA::GetNextAbilityLevelDescription(int32 AbilityLevel)
{
	return FString::Printf(TEXT("<Default>Next Level: </><Level>%d</> \n<Default> 에 도달하면 더 큰 데미지 피해를 입힐 수 있습니다. </>"), AbilityLevel);
}

FString UTDGA::GetLockedDescription(int32 AbilityLevel)
{
	return FString::Printf(TEXT("<Default> %d 레벨에 도달하면 스킬이 해금됩니다. </>"), AbilityLevel);
}

float UTDGA::GetManaCost(float InAbilityLevel) const
{
	float ManaCost = 0.f;
	if (const UGameplayEffect* CostEffect = GetCostGameplayEffect())
	{
		for (FGameplayModifierInfo ModifierInfoIter : CostEffect->Modifiers)
		{
			if (ModifierInfoIter.Attribute == UTDAttributeSet::GetManaAttribute())
			{
				ModifierInfoIter.ModifierMagnitude.GetStaticMagnitudeIfPossible(InAbilityLevel, ManaCost);
				break;
			}
		}
	}

	return ManaCost;
}

float UTDGA::GetCooldown(float InAbilityLevel) const
{
	float Cooldown = 0.f;
	if (const UGameplayEffect* CooldownEffect = GetCooldownGameplayEffect())
	{
		CooldownEffect->DurationMagnitude.GetStaticMagnitudeIfPossible(InAbilityLevel, Cooldown);
	}

	return Cooldown;
}
