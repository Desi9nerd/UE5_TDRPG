#include "GAS/GameplayAbility/TDGA_DamageFrozenField.h"

#include "Interface/IPlayer.h"

void UTDGA_DamageFrozenField::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (false == CheckCost(Handle, ActorInfo)) return;

	IIPlayer* PlayerInterface = Cast<IIPlayer>(GetAvatarActorFromActorInfo());
	PlayerInterface->ShowDecal();
	
}
