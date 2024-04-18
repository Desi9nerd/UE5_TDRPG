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
	DOREPLIFETIME(ATDPlayerState, AttributePoints);
	DOREPLIFETIME(ATDPlayerState, SkillPoints);
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

void ATDPlayerState::OnRep_PlayerLevel()
{
	OnPlayerLevelChangedDelegate.Broadcast(PlayerLevel);
}

void ATDPlayerState::OnRep_Exp()
{
	OnExpChangedDelegate.Broadcast(Exp);
}

void ATDPlayerState::OnRep_AttributePoints(int32 InNewAttributePoints)
{
	AttributePoints += InNewAttributePoints;
	OnAttributePointsChangedDelegate.Broadcast(AttributePoints);
}

void ATDPlayerState::OnRep_SkillPoints(int32 InNewSkillPoints)
{
	SkillPoints += InNewSkillPoints;
	OnSkillPointsChangedDelegate.Broadcast(SkillPoints);
}
