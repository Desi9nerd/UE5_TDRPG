#include "Player/TDPlayerState.h"
#include "GAS/TDAbilitySystemComponent.h"
#include "GAS/TDAttributeSet.h"
#include "Net/UnrealNetwork.h"

ATDPlayerState::ATDPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UTDAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UTDAttributeSet>(TEXT("AttributeSet"));

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

void ATDPlayerState::BeginPlay()
{
	Super::BeginPlay();

	// Ability System Component 초기화가 완료되면 델리게이트 호출
	if (AbilitySystemComponent)
	{
		OnAbilitySystemComponentInitialized.Broadcast();
	}
}

UAbilitySystemComponent* ATDPlayerState::GetAbilitySystemComponent() const
{
	if(false == IsValid(AbilitySystemComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("AbilitySystemComponent NULL!  Check UAbilitySystemComponent* ATDPlayerState::GetAbilitySystemComponent()"));
	}

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

void ATDPlayerState::AddToAttributePoints(int32 InNewAttributePoints)
{
	AttributePoints += InNewAttributePoints;
	OnAttributePointsChangedDelegate.Broadcast(AttributePoints);
}

void ATDPlayerState::AddToSkillPoints(int32 InNewSkillPoints)
{
	SkillPoints += InNewSkillPoints;
	OnSkillPointsChangedDelegate.Broadcast(SkillPoints);
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

void ATDPlayerState::SetAttributePoints(int32 InAttributePoints)
{
	AttributePoints = InAttributePoints;
	OnAttributePointsChangedDelegate.Broadcast(AttributePoints);
}

void ATDPlayerState::SetSkillPoints(int32 InSkillPoints)
{
	SkillPoints = InSkillPoints;
	OnSkillPointsChangedDelegate.Broadcast(SkillPoints);
}

void ATDPlayerState::OnRep_PlayerLevel()
{
	OnPlayerLevelChangedDelegate.Broadcast(PlayerLevel);
}

void ATDPlayerState::OnRep_Exp()
{
	OnExpChangedDelegate.Broadcast(Exp);
}

void ATDPlayerState::OnRep_AttributePoints()
{
	OnAttributePointsChangedDelegate.Broadcast(AttributePoints);
}

void ATDPlayerState::OnRep_SkillPoints()
{
	OnSkillPointsChangedDelegate.Broadcast(SkillPoints);
}
