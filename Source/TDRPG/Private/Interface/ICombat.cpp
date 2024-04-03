#include "Interface/ICombat.h"

int32 IICombat::GetPlayerLevel()
{
	return 0;
}

FVector IICombat::GetCombatSocketLocationCPP(const FGameplayTag& MontageTag)
{
	return FVector();
}

void IICombat::SetFacingTarget(const FVector& FacingTarget)
{
}

void IICombat::Die()
{
}

AActor* IICombat::GetCombatTargetCPP() const
{
	return nullptr;
}

TArray<FTaggedMontage> IICombat::GetAttackMontagesCPP()
{
	return TArray<FTaggedMontage>();
}
