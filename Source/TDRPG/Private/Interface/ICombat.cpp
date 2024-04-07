#include "Interface/ICombat.h"

#include "NiagaraSystem.h"

int32 IICombat::GetPlayerLevel()
{
	return 0;
}

void IICombat::SetFacingTarget(const FVector& FacingTarget)
{
}

void IICombat::Die()
{
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
