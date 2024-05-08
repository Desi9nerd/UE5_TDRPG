#include "GAS/GameplayAbility/TDGA_DamageProjectile_Fireball.h"
#include "GameplayTags/TDGameplayTags.h"

FString UTDGA_DamageProjectile_Fireball::GetDescription(int32 AbilityLevel)
{
	const float ScaledDamage = DamageScalableFloat.GetValueAtLevel(AbilityLevel);
	const float ManaCost = FMath::Abs(GetManaCost(AbilityLevel));
	const float Cooldown = GetCooldown(AbilityLevel);

	if (AbilityLevel == 1)
	{
		return FString::Printf(TEXT(
			// Title
			"<Title>파이어볼</>\n\n"
			// Description
			"<Default>파이어볼을 날려 적에게 피해를 입힙니다.</>\n\n"
			// Level
			"<Small>스킬 레벨: </><Level>%d</>\n"
			// ManaCost
			"<Small>마나 소모량: </><ManaCost>%.f</>\n"
			// Cooldown
			"<Small>쿨타임: </><Cooldown>%.1f</>\n"
			// Damage
			"<Small>데미지 피해량: </><Damage>%.f</>\n"),
			// Values
			AbilityLevel, ManaCost,	Cooldown, ScaledDamage);
	}

	return FString::Printf(TEXT(
		// Title
		"<Title>파이어볼</>\n\n"
		// Description. NumProjectiles
		"<Default> %d 개의 파이어볼을 날려 적에게 피해를 입힙니다.</>\n\n"
		// Level
		"<Small>스킬 레벨: </><Level>%d</>\n"
		// ManaCost
		"<Small>마나 소모량: </><ManaCost>%.f</>\n"
		// Cooldown
		"<Small>쿨타임: </><Cooldown>%.1f</>\n"
		"<Default>파이어볼을 날려 적에게 피해를 입힙니다.</>\n"
		// Damage
		"<Small>데미지 피해량: </><Damage>%.f</>\n"),
		// Values
		AbilityLevel, FMath::Min(AbilityLevel, NumProjectiles), ManaCost, Cooldown, ScaledDamage);
}

FString UTDGA_DamageProjectile_Fireball::GetNextAbilityLevelDescription(int32 AbilityLevel)
{
	const float ScaledDamage = DamageScalableFloat.GetValueAtLevel(AbilityLevel);
	const float ManaCost = FMath::Abs(GetManaCost(AbilityLevel));
	const float Cooldown = GetCooldown(AbilityLevel);

	return FString::Printf(TEXT(
		// Title
		"<Title>다음 스킬레벨</>\n\n"
		// Description. NumProjectiles
		"<Default> %d 개의 파이어볼을 날려 적에게 피해를 입힙니다.</>\n\n"
		// Level
		"<Small>스킬 레벨: </><Level>%d</>\n"
		// ManaCost
		"<Small>마나 소모량: </><ManaCost>%.f</>\n"
		// Cooldown
		"<Small>쿨타임: </><Cooldown>%.1f</>\n"
		// Damage
		"<Small>데미지 피해량: </><Damage>%.f</>\n"),
		// Values
		AbilityLevel, FMath::Min(AbilityLevel, NumProjectiles), ManaCost, Cooldown, ScaledDamage);
}