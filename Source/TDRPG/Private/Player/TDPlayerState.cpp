#include "Player/TDPlayerState.h"
#include "GAS/TDAbilitySystemComponent.h"
#include "GAS/TDAttributeSet.h"
#include "Net/UnrealNetwork.h"

ATDPlayerState::ATDPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UTDAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UTDAttributeSet>("AttributeSet");

	NetUpdateFrequency = 100.f;
}

void ATDPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATDPlayerState, PlayerLevel);
	DOREPLIFETIME(ATDPlayerState, Exp);
}

UAbilitySystemComponent* ATDPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ATDPlayerState::AddToPlayerLevel(int32 InPlayerLevel)
{
	PlayerLevel += InPlayerLevel;
	OnPlayerLevelChangedDelegate.Broadcast(PlayerLevel);
}

void ATDPlayerState::AddToExp(int32 InExp)
{
	Exp += InExp;
	OnExpChangedDelegate.Broadcast(Exp);
}

void ATDPlayerState::SetPlayerLevel(int32 InPlayerLevel)
{
	PlayerLevel = InPlayerLevel;
	OnPlayerLevelChangedDelegate.Broadcast(PlayerLevel);
}

void ATDPlayerState::SetExp(int32 InExp)
{
	Exp = InExp;
	OnExpChangedDelegate.Broadcast(Exp);
}

void ATDPlayerState::OnRep_PlayerLevel(int32 OldPlayerLevel)
{
	OnPlayerLevelChangedDelegate.Broadcast(PlayerLevel);
}

void ATDPlayerState::OnRep_Exp(int32 OldExp)
{
	OnExpChangedDelegate.Broadcast(Exp);
}
