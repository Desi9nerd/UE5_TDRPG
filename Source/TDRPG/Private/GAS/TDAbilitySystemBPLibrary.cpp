#include "GAS/TDAbilitySystemBPLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "UI/WidgetController/TDWidgetController.h"
#include "Player/TDPlayerState.h"
#include "UI/HUD/TDHUD.h"
#include "GameMode/TDGameModeBase.h"
#include "GAS/GameplayEffectContext/TDAbilityTypes.h"
#include "Interface/ICombat.h"

UTDWidgetControllerOverlay* UTDAbilitySystemBPLibrary::GetWidgetControllerOverlay(const UObject* WorldContextObject)
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (ATDHUD* tdHUD = Cast<ATDHUD>(PlayerController->GetHUD()))
		{
			ATDPlayerState* PS = PlayerController->GetPlayerState<ATDPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PlayerController, PS, ASC, AS);

			return tdHUD->GetWidgetControllerOverlay(WidgetControllerParams);
		}
	}
	return nullptr;
}

UTDWidgetControllerAttributeMenu* UTDAbilitySystemBPLibrary::GetAttributeMenuWidgetController(
	const UObject* WorldContextObject)
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (ATDHUD* tdHUD = Cast<ATDHUD>(PlayerController->GetHUD()))
		{
			ATDPlayerState* PS = PlayerController->GetPlayerState<ATDPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PlayerController, PS, ASC, AS);

			return tdHUD->GetWidgetControllerAttributeMenu(WidgetControllerParams);
		}
	}
	return nullptr;
}

void UTDAbilitySystemBPLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC)
{
	AActor* AvatarActor = ASC->GetAvatarActor(); // GameplayEffect가 적용될 Source

	UTDDA_CharacterClass* DataAssetCharacterClass = GetTDDA_CharacterClass(WorldContextObject);
	FCharacterClassDefaultInfo ClassDefaultInfo = DataAssetCharacterClass->GetClassDefaultInfo(CharacterClass);

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

	const FCharacterClassDefaultInfo& CharacterClassDefaultInfo = TDDACharacterClass->GetClassDefaultInfo(CharacterClass);
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
	TObjectPtr<ATDGameModeBase> TDGameMode = Cast<ATDGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (false == IsValid(TDGameMode)) return nullptr;

	check(TDGameMode->TDDACharacterClass);
	return TDGameMode->TDDACharacterClass;
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
			const bool IsDead = IICombat::Execute_IsDead(Overlap.GetActor());
			if (ImplementsCombatInterface && false == IsDead)
			{
				OutOverlappingActors.AddUnique(IICombat::Execute_GetAvatar(Overlap.GetActor()));
			}
		}
	}
}
