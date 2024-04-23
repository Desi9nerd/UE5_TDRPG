#include "GAS/TDAbilitySystemBPLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "UI/WidgetController/TDWidgetController.h"
#include "Player/TDPlayerState.h"
#include "UI/HUD/TDHUD.h"
#include "GameMode/TDGameModeBase.h"
#include "GAS/GameplayEffectContext/TDAbilityTypes.h"
#include "Interface/ICombat.h"

bool UTDAbilitySystemBPLibrary::MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& ResultWCParams, ATDHUD*& OutTDHUD)
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		OutTDHUD = Cast<ATDHUD>(PlayerController->GetHUD());
		if (IsValid(OutTDHUD))
		{
			ATDPlayerState* TDPlayerState = PlayerController->GetPlayerState<ATDPlayerState>();
			UAbilitySystemComponent* ASC = TDPlayerState->GetAbilitySystemComponent();
			UAttributeSet* AttributeSet = TDPlayerState->GetAttributeSet();

			// 값 넣기
			ResultWCParams.AttributeSet = AttributeSet;
			ResultWCParams.AbilitySystemComponent = ASC;
			ResultWCParams.PlayerState = TDPlayerState;
			ResultWCParams.PlayerController = PlayerController;

			return true;
		}
	}

	return false;
}

UTDWidgetControllerOverlay* UTDAbilitySystemBPLibrary::GetTDWidgetControllerOverlay(const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	ATDHUD* TDHUD = nullptr;
	if (MakeWidgetControllerParams(WorldContextObject, WCParams, TDHUD))
	{
		return TDHUD->GetTDWidgetControllerOverlay(WCParams);
	}

	return nullptr;
}

UTDWidgetControllerAttributeMenu* UTDAbilitySystemBPLibrary::GetTDWidgetControllerAttributeMenu(const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	ATDHUD* TDHUD = nullptr;
	if (MakeWidgetControllerParams(WorldContextObject, WCParams, TDHUD))
	{
		return TDHUD->GetTDWidgetControllerAttributeMenu(WCParams);
	}

	return nullptr;
}

UTDWidgetControllerSkillMenu* UTDAbilitySystemBPLibrary::GetTDWidgetControllerSKillMenu(const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	ATDHUD* TDHUD = nullptr;
	if (MakeWidgetControllerParams(WorldContextObject, WCParams, TDHUD))
	{
		return TDHUD->GetTDWidgetControllerSkillMenu(WCParams);
	}

	return nullptr;
}

void UTDAbilitySystemBPLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC)
{
	AActor* AvatarActor = ASC->GetAvatarActor(); // GameplayEffect가 적용될 Source

	UTDDA_CharacterClass* DataAssetCharacterClass = GetTDDA_CharacterClass(WorldContextObject);
	FDA_CharacterClass ClassDefaultInfo = DataAssetCharacterClass->GetDA_ClassClass(CharacterClass);

	FGameplayEffectContextHandle StatAttributesContextHandle = ASC->MakeEffectContext();
	StatAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle StatAttributesSpecHandle = ASC->MakeOutgoingSpec(ClassDefaultInfo.StatAttributes, Level, StatAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*StatAttributesSpecHandle.Data.Get());

	FGameplayEffectContextHandle SecondaryAttributesContextHandle = ASC->MakeEffectContext();
	SecondaryAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle SecondaryAttributesSpecHandle = ASC->MakeOutgoingSpec(DataAssetCharacterClass->SecondaryAttributes, Level, SecondaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSpecHandle.Data.Get());

	FGameplayEffectContextHandle VitalAttributesContextHandle = ASC->MakeEffectContext();
	VitalAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle VitalAttributesSpecHandle = ASC->MakeOutgoingSpec(DataAssetCharacterClass->VitalAttributes, Level, VitalAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributesSpecHandle.Data.Get());
}

void UTDAbilitySystemBPLibrary::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass)
{
	TObjectPtr<UTDDA_CharacterClass> TDDACharacterClass = GetTDDA_CharacterClass(WorldContextObject);
	if (false == IsValid(TDDACharacterClass)) return;

	for (TSubclassOf<UGameplayAbility> AbilityClass : TDDACharacterClass->CommonAbilities)
	{
		// CommonAbilities(ex.HitReact, Die)는 레벨이 변경되도 변경되지 않으므로 InLevel를 1로 하드코딩함.
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		ASC->GiveAbility(AbilitySpec);
	}

	const FDA_CharacterClass& CharacterClassDefaultInfo = TDDACharacterClass->GetDA_ClassClass(CharacterClass);
	for (TSubclassOf<UGameplayAbility> AbilityClass : CharacterClassDefaultInfo.StartupAbilities)
	{
		IICombat* CombatInterface = Cast<IICombat>(ASC->GetAvatarActor());
		if (CombatInterface)
		{
			// PlayerLevel에 따라 Ability가 달라지므로 PlayerLevel를 변수로 넘김.
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, CombatInterface->GetPlayerLevel());
			ASC->GiveAbility(AbilitySpec);
		}
	}
}

UTDDA_CharacterClass* UTDAbilitySystemBPLibrary::GetTDDA_CharacterClass(const UObject* WorldContextObject)
{
	const ATDGameModeBase* TDGameMode = Cast<ATDGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (false == IsValid(TDGameMode)) return nullptr;
	checkf(TDGameMode->TDDACharacterClass, TEXT("No TDGameMode. Check: UTDAbilitySystemBPLibrary::GetTDDA_CharacterClass"));

	return TDGameMode->TDDACharacterClass;
}

UTDDA_Ability* UTDAbilitySystemBPLibrary::GetTDDA_Ability(const UObject* WorldContextObject)
{
	const ATDGameModeBase* TDGameMode = Cast<ATDGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	//if (false == IsValid(TDGameMode)) return nullptr;
	checkf(TDGameMode, TEXT("No TDGameMode. Check: UTDAbilitySystemBPLibrary::GetTDDA_Ability"));
	checkf(TDGameMode->TDDAAbility, TEXT("No TDAbility. Check: UTDAbilitySystemBPLibrary::GetTDDA_Ability"));

	return TDGameMode->TDDAAbility;
}

bool UTDAbilitySystemBPLibrary::IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	const FTDGameplayEffectContext* TDGameplayEffectContext = static_cast<const FTDGameplayEffectContext*>(EffectContextHandle.Get());
	if (TDGameplayEffectContext)
	{
		return TDGameplayEffectContext->IsBlockedHit();
	}
	return false;
}

bool UTDAbilitySystemBPLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	const FTDGameplayEffectContext* TDGameplayEffectContext = static_cast<const FTDGameplayEffectContext*>(EffectContextHandle.Get());
	if (TDGameplayEffectContext)
	{
		return TDGameplayEffectContext->IsCriticalHit();
	}
	return false;
}

void UTDAbilitySystemBPLibrary::SetIsBlockedHit(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockedHit)
{
	FTDGameplayEffectContext* TDGameplayEffectContext = static_cast<FTDGameplayEffectContext*>(EffectContextHandle.Get());
	if (TDGameplayEffectContext)
	{
		TDGameplayEffectContext->SetIsCriticalHit(bInIsBlockedHit);
	}
}

void UTDAbilitySystemBPLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& EffectContextHandle,
	bool bInIsCriticalHit)
{
	FTDGameplayEffectContext* TDGameplayEffectContext = static_cast<FTDGameplayEffectContext*>(EffectContextHandle.Get());
	if (TDGameplayEffectContext)
	{
		TDGameplayEffectContext->SetIsCriticalHit(bInIsCriticalHit);
	}
}

void UTDAbilitySystemBPLibrary::GetLivePlayersWithinRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin)
{
	//* UGamplayStatics::ApplyRadialDamageWithFallOff 함수와 유사하게 구현. *//

	FCollisionQueryParams SphereParams;
	SphereParams.AddIgnoredActors(ActorsToIgnore);

	const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (IsValid(World))
	{
		TArray<FOverlapResult> Overlaps;
		World->OverlapMultiByObjectType(Overlaps, SphereOrigin, FQuat::Identity, FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects), FCollisionShape::MakeSphere(Radius), SphereParams);
		for (FOverlapResult& Overlap : Overlaps)
		{
			const bool ImplementsCombatInterface = Overlap.GetActor()->Implements<UICombat>();
			if (ImplementsCombatInterface)
			{
				const bool IsDead = IICombat::Execute_IsDead(Overlap.GetActor());
				if(false == IsDead)
				{
					OutOverlappingActors.AddUnique(IICombat::Execute_GetAvatar(Overlap.GetActor()));					
				}
			}
		}
	}
}

bool UTDAbilitySystemBPLibrary::IsSameTeam(AActor* FirstActor, AActor* SecondActor)
{
	if (FirstActor->ActorHasTag("Player") && SecondActor->ActorHasTag("Player") || 
		FirstActor->ActorHasTag("Enemy") && SecondActor->ActorHasTag("Enemy"))
	{
		return true;
	}

	return false;
}

int32 UTDAbilitySystemBPLibrary::GetExpRewardForClassAndLevel(const UObject* WorldContextObject, ECharacterClass CharacterClass, int32 CharacterLevel)
{
	TObjectPtr<UTDDA_CharacterClass> TDDA_CharacterClass = GetTDDA_CharacterClass(WorldContextObject);
	if (false == IsValid(TDDA_CharacterClass)) return 0;

	const FDA_CharacterClass& DA_CharacterClassInfo = TDDA_CharacterClass->GetDA_ClassClass(CharacterClass);
	const float ExpReward = DA_CharacterClassInfo.ExpReward.GetValueAtLevel(CharacterLevel);

	return static_cast<int32>(ExpReward);
}
