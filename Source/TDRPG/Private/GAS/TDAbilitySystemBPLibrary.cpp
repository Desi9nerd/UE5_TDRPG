#include "GAS/TDAbilitySystemBPLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "SaveGame/TDSaveGame_Load.h"
#include "Kismet/GameplayStatics.h"
#include "UI/WidgetController/TDWidgetController.h"
#include "Player/TDPlayerState.h"
#include "UI/HUD/TDHUD.h"
#include "GameMode/TDGameModeBase.h"
#include "GameplayTags/TDGameplayTags.h"
#include "GAS/GameplayEffectContext/TDAbilityTypes.h"
#include "Interface/ICombat.h"
#include "Library/TDDamageLibrary.h"
#include "Library/TDItemLibrary.h"

void UTDAbilitySystemBPLibrary::InitializeDefaultAttributesFromSaveData(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, UTDSaveGame_Load* SaveGame)
{
	UTDDA_CharacterClass* TDDA_CharacterClass = GetTDDA_CharacterClass(WorldContextObject);
	if (false == IsValid(TDDA_CharacterClass)) return;


	const FTDGameplayTags& GameplayTags = FTDGameplayTags::GetTDGameplayTags();
	const AActor* SourceAvatarActor = ASC->GetAvatarActor();

	FGameplayEffectContextHandle EffectContexthandle = ASC->MakeEffectContext();
	EffectContexthandle.AddSourceObject(SourceAvatarActor);

	//* Stat Attributes
	// Stat Attributes의 SpecHandle
	const FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(TDDA_CharacterClass->StatAttributes_SetByCaller, 1.f, EffectContexthandle);
	// Stat Attributes 할당하기
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Attributes_Stat_Strength, SaveGame->Strength);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Attributes_Stat_Intelligence, SaveGame->Intelligence);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Attributes_Stat_Resilience, SaveGame->Resilience);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Attributes_Stat_Vigor, SaveGame->Vigor);
	ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);

	//* Secondary Attributes
	FGameplayEffectContextHandle SecondaryAttributesContextHandle = ASC->MakeEffectContext();
	SecondaryAttributesContextHandle.AddSourceObject(SourceAvatarActor);
	const FGameplayEffectSpecHandle SecondaryAttributesSpecHandle = ASC->MakeOutgoingSpec(TDDA_CharacterClass->SecondaryAttributes_Infinite, 1.f, SecondaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSpecHandle.Data.Get());

	//* Vital Attributes
	FGameplayEffectContextHandle VitalAttributesContextHandle = ASC->MakeEffectContext();
	VitalAttributesContextHandle.AddSourceObject(SourceAvatarActor);
	const FGameplayEffectSpecHandle VitalAttributesSpecHandle = ASC->MakeOutgoingSpec(TDDA_CharacterClass->VitalAttributes, 1.f, VitalAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributesSpecHandle.Data.Get());
}

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
	//checkf(TDGameMode->TDDACharacterClass, TEXT("No TDGameMode. Check: UTDAbilitySystemBPLibrary::GetTDDA_CharacterClass"));

	return TDGameMode->TDDACharacterClass;
}

UTDDA_Ability* UTDAbilitySystemBPLibrary::GetTDDA_Ability(const UObject* WorldContextObject)
{
	const ATDGameModeBase* TDGameMode = Cast<ATDGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (false == IsValid(TDGameMode)) return nullptr;
	//checkf(TDGameMode, TEXT("No TDGameMode. Check: UTDAbilitySystemBPLibrary::GetTDDA_Ability"));
	//checkf(TDGameMode->TDDAAbility, TEXT("No TDAbility. Check: UTDAbilitySystemBPLibrary::GetTDDA_Ability"));

	return TDGameMode->TDDAAbility;
}

FGameplayTag UTDAbilitySystemBPLibrary::GetDamageType(const FGameplayEffectContextHandle& EffectContextHandle)
{
	const FTDGameplayEffectContext* TDGameplayEffectContext = static_cast<const FTDGameplayEffectContext*>(EffectContextHandle.Get());
	if (TDGameplayEffectContext)
	{
		if (TDGameplayEffectContext->GetDamageType().IsValid())
		{
			return *TDGameplayEffectContext->GetDamageType();
		}
	}
	return FGameplayTag();
}

float UTDAbilitySystemBPLibrary::GetDebuffDamage(const FGameplayEffectContextHandle& EffectContextHandle)
{
	const FTDGameplayEffectContext* TDGameplayEffectContext = static_cast<const FTDGameplayEffectContext*>(EffectContextHandle.Get());
	if (TDGameplayEffectContext)
	{
		return TDGameplayEffectContext->GetDebuffDamage();
	}
	return 0.f;
}

float UTDAbilitySystemBPLibrary::GetDebuffDuration(const FGameplayEffectContextHandle& EffectContextHandle)
{
	const FTDGameplayEffectContext* TDGameplayEffectContext = static_cast<const FTDGameplayEffectContext*>(EffectContextHandle.Get());
	if (TDGameplayEffectContext)
	{
		return TDGameplayEffectContext->GetDebuffDuration();
	}
	return 0.f;
}

float UTDAbilitySystemBPLibrary::GetDebuffFrequency(const FGameplayEffectContextHandle& EffectContextHandle)
{
	const FTDGameplayEffectContext* TDGameplayEffectContext = static_cast<const FTDGameplayEffectContext*>(EffectContextHandle.Get());
	if (TDGameplayEffectContext)
	{
		return TDGameplayEffectContext->GetDebuffFrequency();
	}
	return 0.f;
}

FVector UTDAbilitySystemBPLibrary::GetRagdollImpulse(const FGameplayEffectContextHandle& EffectContextHandle)
{
	const FTDGameplayEffectContext* TDGameplayEffectContext = static_cast<const FTDGameplayEffectContext*>(EffectContextHandle.Get());
	if (TDGameplayEffectContext)
	{
		return TDGameplayEffectContext->GetRagdollImpulse();
	}
	return FVector::ZeroVector;
}

FVector UTDAbilitySystemBPLibrary::GetKnockbackImpulse(const FGameplayEffectContextHandle& EffectContextHandle)
{
	const FTDGameplayEffectContext* TDGameplayEffectContext = static_cast<const FTDGameplayEffectContext*>(EffectContextHandle.Get());
	if (TDGameplayEffectContext)
	{
		return TDGameplayEffectContext->GetKnockbackImpulse();
	}
	return FVector::ZeroVector;
}

float UTDAbilitySystemBPLibrary::GetRadialDamageInnerRadius(const FGameplayEffectContextHandle& EffectContextHandle)
{
	const FTDGameplayEffectContext* TDGameplayEffectContext = static_cast<const FTDGameplayEffectContext*>(EffectContextHandle.Get());
	if (TDGameplayEffectContext)
	{
		return TDGameplayEffectContext->GetRadialDamageInnerRadius();
	}
	return 0.f;
}

float UTDAbilitySystemBPLibrary::GetRadialDamageOuterRadius(const FGameplayEffectContextHandle& EffectContextHandle)
{
	const FTDGameplayEffectContext* TDGameplayEffectContext = static_cast<const FTDGameplayEffectContext*>(EffectContextHandle.Get());
	if (TDGameplayEffectContext)
	{
		return TDGameplayEffectContext->GetRadialDamageOuterRadius();
	}
	return 0.f;
}

FVector UTDAbilitySystemBPLibrary::GetRadialDamageOrigin(const FGameplayEffectContextHandle& EffectContextHandle)
{
	const FTDGameplayEffectContext* TDGameplayEffectContext = static_cast<const FTDGameplayEffectContext*>(EffectContextHandle.Get());
	if (TDGameplayEffectContext)
	{
		return TDGameplayEffectContext->GetRadialDamageOrigin();
	}
	return FVector::ZeroVector;
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

bool UTDAbilitySystemBPLibrary::IsDebuff(const FGameplayEffectContextHandle& EffectContextHandle)
{
	const FTDGameplayEffectContext* TDGameplayEffectContext = static_cast<const FTDGameplayEffectContext*>(EffectContextHandle.Get());
	if (TDGameplayEffectContext)
	{
		return TDGameplayEffectContext->IsDebuff();
	}
	return false;
}

bool UTDAbilitySystemBPLibrary::IsRadialDamage(const FGameplayEffectContextHandle& EffectContextHandle)
{
	const FTDGameplayEffectContext* TDGameplayEffectContext = static_cast<const FTDGameplayEffectContext*>(EffectContextHandle.Get());
	if (TDGameplayEffectContext)
	{
		return TDGameplayEffectContext->IsRadialDamage();
	}
	return false;
}

void UTDAbilitySystemBPLibrary::SetIsRadialDamageEffectParam(FDamageEffectParams& DamageEffectParams, bool bIsRadial, float InnerRadius, float OuterRadius, FVector Origin)
{
	DamageEffectParams.bRadialDamage = bIsRadial;
	DamageEffectParams.RadialDamageInnerRadius = InnerRadius;
	DamageEffectParams.RadialDamageOuterRadius = OuterRadius;
	DamageEffectParams.RadialDamageOrigin = Origin;
}

void UTDAbilitySystemBPLibrary::SetKnockbackDirection(FDamageEffectParams& DamageEffectParams, FVector KnockbackDirection, float Magnitude)
{
	KnockbackDirection.Normalize();
	if (Magnitude == 0.f)
	{
		DamageEffectParams.KnockbackImpulse = KnockbackDirection * DamageEffectParams.KnockbackImpulseMagnitude;
	}
	else
	{
		DamageEffectParams.KnockbackImpulse = KnockbackDirection * Magnitude;
	}
}

void UTDAbilitySystemBPLibrary::SetRagdollImpulseDirection(FDamageEffectParams& DamageEffectParams, FVector ImpulseDirection, float Magnitude)
{
	ImpulseDirection.Normalize();
	if (Magnitude == 0.f)
	{
		DamageEffectParams.RagdollImpulse = ImpulseDirection * DamageEffectParams.RagdollImpulseMagnitude;
	}
	else
	{
		DamageEffectParams.RagdollImpulse = ImpulseDirection * Magnitude;
	}
}

void UTDAbilitySystemBPLibrary::SetTargetEffectParamsASC(FDamageEffectParams& DamageEffectParams, UAbilitySystemComponent* InASC)
{
	DamageEffectParams.TargetAbilitySystemComponent = InASC;
}

void UTDAbilitySystemBPLibrary::SetIsBlockedHit(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockedHit)
{
	FTDGameplayEffectContext* TDGameplayEffectContext = static_cast<FTDGameplayEffectContext*>(EffectContextHandle.Get());
	if (TDGameplayEffectContext)
	{
		TDGameplayEffectContext->SetCriticalHit(bInIsBlockedHit);
	}
}

void UTDAbilitySystemBPLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& EffectContextHandle,
	bool bInIsCriticalHit)
{
	FTDGameplayEffectContext* TDGameplayEffectContext = static_cast<FTDGameplayEffectContext*>(EffectContextHandle.Get());
	if (TDGameplayEffectContext)
	{
		TDGameplayEffectContext->SetCriticalHit(bInIsCriticalHit);
	}
}

void UTDAbilitySystemBPLibrary::SetDebuff(FGameplayEffectContextHandle& EffectContextHandle, bool bInDebuff)
{
	if (FTDGameplayEffectContext* TDGEContext = static_cast<FTDGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		TDGEContext->SetDebuff(bInDebuff);
	}
}

void UTDAbilitySystemBPLibrary::SetDebuffDamage(FGameplayEffectContextHandle& EffectContextHandle, float InDebuffDamage)
{
	if (FTDGameplayEffectContext* TDGEContext = static_cast<FTDGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		TDGEContext->SetDebuffDamage(InDebuffDamage);
	}
}

void UTDAbilitySystemBPLibrary::SetDebuffDuration(FGameplayEffectContextHandle& EffectContextHandle, float InDebuffDuration)
{
	if (FTDGameplayEffectContext* TDGEContext = static_cast<FTDGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		TDGEContext->SetDebuffDuration(InDebuffDuration);
	}
}

void UTDAbilitySystemBPLibrary::SetDebuffFrequency(FGameplayEffectContextHandle& EffectContextHandle, float InDebuffFrequency)
{
	if (FTDGameplayEffectContext* TDGEContext = static_cast<FTDGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		TDGEContext->SetDebuffFrequency(InDebuffFrequency);
	}
}

void UTDAbilitySystemBPLibrary::SetDamageType(FGameplayEffectContextHandle& EffectContextHandle, const FGameplayTag& InDamageType)
{
	if (FTDGameplayEffectContext* TDGEContext = static_cast<FTDGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		const TSharedPtr<FGameplayTag> NewDamageType = MakeShared<FGameplayTag>(InDamageType);
		TDGEContext->SetDamageType(NewDamageType);
	}
}

void UTDAbilitySystemBPLibrary::SetRagdollImpulse(FGameplayEffectContextHandle& EffectContextHandle, const FVector& InRagdollImpulse)
{
	if (FTDGameplayEffectContext* TDGEContext = static_cast<FTDGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		TDGEContext->SetRagdollImpulse(InRagdollImpulse);
	}
}

void UTDAbilitySystemBPLibrary::SetKnockbackImpulse(FGameplayEffectContextHandle& EffectContextHandle, const FVector& InKnockbackImpulse)
{
	if (FTDGameplayEffectContext* TDGEContext = static_cast<FTDGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		TDGEContext->SetKnockbackImpulse(InKnockbackImpulse);
	}
}

void UTDAbilitySystemBPLibrary::SetIsRadialDamage(FGameplayEffectContextHandle& EffectContextHandle, bool InbRadialDamage)
{
	if (FTDGameplayEffectContext* TDGEContext = static_cast<FTDGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		TDGEContext->SetIsRadialDamage(InbRadialDamage);
	}
}

void UTDAbilitySystemBPLibrary::SetRadialDamageInnerRadius(FGameplayEffectContextHandle& EffectContextHandle, float InInnerRadius)
{
	if (FTDGameplayEffectContext* TDGEContext = static_cast<FTDGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		TDGEContext->SetRadialDamageInnerRadius(InInnerRadius);
	}
}

void UTDAbilitySystemBPLibrary::SetRadialDamageOuterRadius(FGameplayEffectContextHandle& EffectContextHandle, float InOuterRadius)
{
	if (FTDGameplayEffectContext* TDGEContext = static_cast<FTDGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		TDGEContext->SetRadialDamageOuterRadius(InOuterRadius);
	}
}

void UTDAbilitySystemBPLibrary::SetRadialDamageOrigin(FGameplayEffectContextHandle& EffectContextHandle, const FVector& InOrigin)
{
	if (FTDGameplayEffectContext* TDGEContext = static_cast<FTDGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		TDGEContext->SetRadialDamageOrigin(InOrigin);
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
			IICombat* CombatInterface = Cast<IICombat>(Overlap.GetActor());
			if (CombatInterface)
			{
				const bool IsDead = CombatInterface->IsDead();
				if(false == IsDead)
				{
					OutOverlappingActors.AddUnique(IICombat::Execute_GetAvatar(Overlap.GetActor()));					
				}
			}
		}
	}
}

void UTDAbilitySystemBPLibrary::GetClosestTargets(int32 MaxTargets, const TArray<AActor*>& InActors, TArray<AActor*>& OutClosestTargets, const FVector& InOrigin)
{
	if (InActors.Num() <= MaxTargets) // 인식된 적들의 수 <= 최대 인식 가능한 적 수
	{
		OutClosestTargets = InActors; // 인식된 적들(=InActors)를 모두 OutClosestTargets에 담음.
		return;
	}

	// 인식된 적들의 수 > 최대 인식 가능한 적 수
	// 가까운 순서로 OutClosestTargets에 담음.
	TArray<AActor*> ActorsToCheck = InActors;
	int32 NumOfTargetsFound = 0;

	while (NumOfTargetsFound < MaxTargets)
	{
		if (ActorsToCheck.Num() == 0) break;

		double ClosestDistance = TNumericLimits<double>::Max(); // 최대값으로 시작.
		AActor* ClosestActor;

		for (AActor* PotentialTarget : ActorsToCheck)
		{
			const double Distance = (PotentialTarget->GetActorLocation() - InOrigin).Length(); // 타겟과 플레이어 사이의 거리

			if (Distance < ClosestDistance) // 가장 가까운 거리 업데이트
			{
				ClosestDistance = Distance;
				ClosestActor = PotentialTarget;
			}
		}

		ActorsToCheck.Remove(ClosestActor);
		OutClosestTargets.AddUnique(ClosestActor);
		NumOfTargetsFound++;
	}
}

UTDDA_DropItem* UTDAbilitySystemBPLibrary::GetTDDA_DropItem(const UObject* WorldContextObject)
{
	const ATDGameModeBase* TDGameMode = Cast<ATDGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (false == IsValid(TDGameMode)) return nullptr;

	return TDGameMode->TDDADropItem;
}

bool UTDAbilitySystemBPLibrary::IsSameTeam(AActor* FirstActor, AActor* SecondActor)
{
	if ( (FirstActor->ActorHasTag("Player") && SecondActor->ActorHasTag("Player")) || (FirstActor->ActorHasTag("Enemy") && SecondActor->ActorHasTag("Enemy")) )
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

FGameplayEffectContextHandle UTDAbilitySystemBPLibrary::ApplyDamageEffect(const FDamageEffectParams& DamageEffectParams)
{
	const FTDGameplayTags& GameplayTags = FTDGameplayTags::GetTDGameplayTags();
	const AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();

	FGameplayEffectContextHandle EffectContexthandle = DamageEffectParams.SourceAbilitySystemComponent->MakeEffectContext();
	EffectContexthandle.AddSourceObject(SourceAvatarActor);
	SetRagdollImpulse(EffectContexthandle, DamageEffectParams.RagdollImpulse);
	SetKnockbackImpulse(EffectContexthandle, DamageEffectParams.KnockbackImpulse);

	SetIsRadialDamage(EffectContexthandle, DamageEffectParams.bRadialDamage);
	SetRadialDamageInnerRadius(EffectContexthandle, DamageEffectParams.RadialDamageInnerRadius);
	SetRadialDamageOuterRadius(EffectContexthandle, DamageEffectParams.RadialDamageOuterRadius);
	SetRadialDamageOrigin(EffectContexthandle, DamageEffectParams.RadialDamageOrigin);

	const FGameplayEffectSpecHandle SpecHandle = DamageEffectParams.SourceAbilitySystemComponent->MakeOutgoingSpec(DamageEffectParams.GEDamageClass, DamageEffectParams.AbilityLevel, EffectContexthandle);

	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, DamageEffectParams.DamageType, DamageEffectParams.BaseDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Chance, DamageEffectParams.DebuffChance);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Damage, DamageEffectParams.DebuffDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Duration, DamageEffectParams.DebuffDuration);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Frequency, DamageEffectParams.DebuffFrequency);

	DamageEffectParams.TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
	return EffectContexthandle;
}

bool UTDAbilitySystemBPLibrary::GetItemInfo(const FString& InItemName, FItem& OutItemInfo)
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableRef(TEXT("/Game/BP/Inventory/Data/DT_ItemInfo.DT_ItemInfo'"));

	if (const UDataTable* ItemDataTable = DataTableRef.Object)
	{
		FItem* ItemInfo = ItemDataTable->FindRow<FItem>(FName(*InItemName), FString("No DataTable. Check UTDAbilitySystemBPLibrary::GetItemInfo"));
		if (ItemInfo != nullptr)
		{
			OutItemInfo = *ItemInfo;
			return true;
		}
	}

	return false;
}

bool UTDAbilitySystemBPLibrary::GetInventoryCategoryInfo(const FString& InItemCategoryName, FInventoryCategory& OutCategoryInfo)
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableRef(TEXT("/Game/BP/Inventory/Data/DT_InventoryCategory.DT_InventoryCategory'"));

	if (const UDataTable* InventoryCategoryDataTable = DataTableRef.Object)
	{
		FInventoryCategory* InventoryCategoryInfo = InventoryCategoryDataTable->FindRow<FInventoryCategory>(FName(*InItemCategoryName), FString("No DataTable. Check UTDAbilitySystemBPLibrary::GetItemInfo"));
		if (InventoryCategoryInfo != nullptr)
		{
			OutCategoryInfo = *InventoryCategoryInfo;
			return true;
		}
	}

	return false;
}
