#include "Interface/ICombat.h"

#include "NiagaraSystem.h"

int32 IICombat::GetPlayerLevel()
{
	return 0;
}

FVector IICombat::GetCombatSocketLocationCPP(const FGameplayTag& MontageTag)
{
	return FVector();
}

void IICombat::UpdateFacingTargetCPP(const FVector& FacingTarget)
{
}

UAnimMontage* IICombat::GetHitReactMontageCPP()
{
	return nullptr;
}

bool IICombat::IsDead() const
{
	return false;
}

AActor* IICombat::GetCombatTargetCPP() const
{
	return nullptr;
}

TArray<FTaggedMontage> IICombat::GetAttackMontagesCPP()
{
	return TArray<FTaggedMontage>();
}

UNiagaraSystem* IICombat::GetBloodEffectCPP()
{
	return nullptr;
}

FTaggedMontage IICombat::GetTaggedMontageByTagCPP(const FGameplayTag& MontageTag)
{
	return FTaggedMontage();
}

int32 IICombat::GetMinionCountCPP()
{
	return 0;
}

void IICombat::IncremenetMinionCntCPP(int32 Amount)
{
}

ECharacterClass IICombat::GetCharacterClassCPP()
{
	return ECharacterClass();
}
