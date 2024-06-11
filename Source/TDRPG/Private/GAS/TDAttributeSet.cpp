#include "GAS/TDAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameFramework/Character.h"
#include "GameplayEffectExtension.h"
#include "GameplayTags/TDGameplayTags.h"
#include "GAS/TDAbilitySystemBPLibrary.h"
#include "GAS/GameplayEffectContext/TDAbilityTypes.h"
#include "Interface/ICombat.h"
#include "Interface/IPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Player/TDPlayerController.h"
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"

UTDAttributeSet::UTDAttributeSet()
{
	//** #define ATTRIBUTE_ACCESSORS 매크로와 연동. 초기값 설정 가능.

	// TagsToAttributes라는 map변수에 key: GameplayTags, value: Attributes값을 넣어준다.
	const FTDGameplayTags& GameplayTags = FTDGameplayTags::GetTDGameplayTags();

	// Stat Attributes
	TagsToAttributes.Add(GameplayTags.Attributes_Stat_Strength, GetStrengthAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Stat_Intelligence, GetIntelligenceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Stat_Resilience, GetResilienceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Stat_Vigor, GetVigorAttribute);
	// Secondary Attributes
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_Armor, GetArmorAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_ArmorPenetration, GetArmorPenetrationAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_BlockChance, GetBlockChanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_CriticalHitChance, GetCriticalHitChanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_CriticalHitResistance, GetCriticalHitResistanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_CriticalHitDamage, GetCriticalHitDamageAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_HealthRegeneration, GetHealthRegenerationAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_ManaRegeneration, GetManaRegenerationAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_MaxHealth, GetMaxHealthAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_MaxMana, GetMaxManaAttribute);
	
	// Resistance Attributes
	TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Physical, GetPhysicalResistanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Fire, GetFireResistanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Ice, GetIceResistanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Electronic, GetElectronicResistanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Ultimate, GetElectronicResistanceAttribute);
}

void UTDAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//** 항상 모두에게 Replicated되어야 하는 것들은 COND_None, REPNOTIFY_Always로 설정
	DOREPLIFETIME_CONDITION_NOTIFY(UTDAttributeSet, Strength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTDAttributeSet, Intelligence, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTDAttributeSet, Resilience, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTDAttributeSet, Vigor, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UTDAttributeSet, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTDAttributeSet, ArmorPenetration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTDAttributeSet, BlockChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTDAttributeSet, CriticalHitChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTDAttributeSet, CriticalHitDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTDAttributeSet, CriticalHitResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTDAttributeSet, HealthRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTDAttributeSet, ManaRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTDAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTDAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UTDAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTDAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTDAttributeSet, Soul, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UTDAttributeSet, PhysicalResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTDAttributeSet, FireResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTDAttributeSet, IceResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTDAttributeSet, ElectronicResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTDAttributeSet, UltimateResistance, COND_None, REPNOTIFY_Always);
}

/** Called just before modifying the value of an attribute. AttributeSet can make additional modifications here. Return true to continue, or false to throw out the modification.*/
// PreAttributeChange함수는 Attribute의 변수 변경 전에 관찰하여 범위를 설정해둔 값이 범위를 벗어났을시 적용되지 않게 한다. Clamp 용도로만 사용하는걸 권장한다.
void UTDAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	//** 수치 Clamp 범위 주기 (attribute 변경 전에 clamp 적용)
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
	}
	if (Attribute == GetSoulAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, 100.f);
	}
}

/**	Called just before a GameplayEffect is executed to modify the base value of an attribute. No more changes can be made.*/
void UTDAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FEffectProperties Props;
	SetEffectProperties(Data, Props); // 이펙트 속성 설정하기

	IICombat* CombatInterface = Cast<IICombat>(Props.TargetCharacter);
	if (CombatInterface && CombatInterface->IsDead()) return; // GameplayEffect를 적용해야하는 상대방이 죽은 경우 바로 리턴.


	//** 수치 Clamp 범위 주기 (attribute 변경 후에 clamp 적용)
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
		//UE_LOG(LogTemp, Warning, TEXT("Changed Health on %s, Health: %f"), *Props.TargetAvatarActor->GetName(), GetHealth());
	}
	if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.f, GetMaxMana()));
	}

	//** 데미지 처리
	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		ApplyIncomingDamage(Props);
	}

	//** 경험치 처리
	if (Data.EvaluatedData.Attribute == GetIncomingExpAttribute())
	{
		ApplyIncomingExp(Props);
	}
}

/** Called just after any modification happens to an attribute. */
void UTDAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if (bMakeMaxHealth && Attribute == GetMaxHealthAttribute())
	{
		SetHealth(GetMaxHealth());
		bMakeMaxHealth = false;
	}
	if (bMakeMaxMana && Attribute == GetMaxManaAttribute())
	{
		SetMana(GetMaxMana());
		bMakeMaxMana = false;
	}
}


void UTDAttributeSet::ApplyIncomingDamage(const FEffectProperties& Props)
{
	const float LocalIncomingDamage = GetIncomingDamage(); // 변수에 들어오는 데미지 저장
	SetIncomingDamage(0.f);

	if (LocalIncomingDamage > 0.f)
	{
		const float NewHealth = GetHealth() - LocalIncomingDamage; // New체력 = 현재 체력 - 데미지 계산
		SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth())); // 체력 업데이트

		const bool bFatal = NewHealth <= 0.f; // 체력이 0이하면 true
		if (bFatal) // 체력 0 이하
		{
			IICombat* CombatInterface = Cast<IICombat>(Props.TargetAvatarActor);
			if (CombatInterface)
			{
				CombatInterface->Die(UTDAbilitySystemBPLibrary::GetRagdollImpulse(Props.EffectContextHandle)); // 캐릭터 사망 처리. RagdollImpulse값도 같이 보냄.
			}
			SendExpEvent(Props); // 경험치 보내기
		}
		else // 체력 0 초과
		{
			//* 데미지 적용하기  *//
			IICombat* CombatInterface = Cast<IICombat>(Props.TargetCharacter);
			if (CombatInterface && false == CombatInterface->BeingShocked()) // Shocked 상태가 아닌지 체크 (Shocked 상태 시 데미지 적용X)
			{
				FGameplayTagContainer TagContainer;
				TagContainer.AddTag(FTDGameplayTags::GetTDGameplayTags().Effect_HitReact);
				Props.TargetASC->TryActivateAbilitiesByTag(TagContainer); // Effect_HitReact 태그가 있는 모든 GameplayAbility들을 활성화
			}

			//* Knockback 적용하기 *//
			const FVector& KnockbackImpulse = UTDAbilitySystemBPLibrary::GetKnockbackImpulse(Props.EffectContextHandle);
			if (false == KnockbackImpulse.IsNearlyZero(1.f)) // 값이 0이 아니라면
			{
				Props.TargetCharacter->LaunchCharacter(KnockbackImpulse, true, true); // 상대방에 Knockback 적용.
			}
		}

		const bool bBlock = UTDAbilitySystemBPLibrary::IsBlockedHit(Props.EffectContextHandle);
		const bool bCriticalHit = UTDAbilitySystemBPLibrary::IsCriticalHit(Props.EffectContextHandle);
		ShowFloatingText(Props, LocalIncomingDamage, bBlock, bCriticalHit); // 데미지 숫자 띄우기. FEffectProperties, 데미지, Block, CriticalHit 정보 전달

		if (UTDAbilitySystemBPLibrary::IsDebuff(Props.EffectContextHandle))
		{
			ApplyDebuff(Props); // 상대방에 디버프 적용.
		}
	}
}

// BP에서 생성한 GameplayEffect 클래스를 적용하려면 동적으로 적용해야 한다.
// 동적으로 추가하면 replication이 적용되지 않으므로 서버에서 실행한 후 변경사항이 발생하면 replicated되어 클라이언트에 알려줘야 한다.
void UTDAttributeSet::ApplyDebuff(const FEffectProperties& Props)
{
	const FTDGameplayTags& TDGameplayTags = FTDGameplayTags::GetTDGameplayTags();
	FGameplayEffectContextHandle GEContextHandle = Props.SourceASC->MakeEffectContext();
	GEContextHandle.AddSourceObject(Props.SourceAvatarActor);

	const FGameplayTag DamageType = UTDAbilitySystemBPLibrary::GetDamageType(Props.EffectContextHandle);
	const float DebuffDamage = UTDAbilitySystemBPLibrary::GetDebuffDamage(Props.EffectContextHandle);
	const float DebuffDuration = UTDAbilitySystemBPLibrary::GetDebuffDuration(Props.EffectContextHandle);
	const float DebuffFrequency = UTDAbilitySystemBPLibrary::GetDebuffFrequency(Props.EffectContextHandle);

	// GameplayEffect 동적 생성.
	FString DebuffName = FString::Printf(TEXT("DynamicDebuff_%s"), *DamageType.ToString());
	UGameplayEffect* GE = NewObject<UGameplayEffect>(GetTransientPackage(), FName(DebuffName));

	GE->DurationPolicy = EGameplayEffectDurationType::HasDuration;
	GE->Period = DebuffFrequency;
	GE->DurationMagnitude = FScalableFloat(DebuffDuration);

	// Set the target tags using UTargetTagsGameplayEffectComponent
	UTargetTagsGameplayEffectComponent* TargetTagsComponent = NewObject<UTargetTagsGameplayEffectComponent>(GE);
	FGameplayTagContainer TagContainer;

	// Add debuff tag
	// DamageType태크 key에 대응하는 value(=Debuff 태그)를 담음.
	const FGameplayTag DebuffTag = FTDGameplayTags::GetTDGameplayTags().DamageTypesToDebuffs[DamageType];

	TagContainer.AddTag(DebuffTag);
	if (DebuffTag.MatchesTagExact(FTDGameplayTags::GetTDGameplayTags().Debuff_Stun))
	{
		TagContainer.AddTag(FTDGameplayTags::GetTDGameplayTags().BlockTag_CursorTrace);
		TagContainer.AddTag(FTDGameplayTags::GetTDGameplayTags().BlockTag_InputHeld);
		TagContainer.AddTag(FTDGameplayTags::GetTDGameplayTags().BlockTag_InputPressed);
		TagContainer.AddTag(FTDGameplayTags::GetTDGameplayTags().BlockTag_InputReleased);
	}
	TagContainer.AddTag(TDGameplayTags.DamageTypesToDebuffs[DamageType]);

	TargetTagsComponent->GetConfiguredTargetTagChanges().ApplyTo(TagContainer);

	GE->StackingType = EGameplayEffectStackingType::AggregateBySource;
	GE->StackLimitCount = 1;

	const int32 Index = GE->Modifiers.Num();
	GE->Modifiers.Add(FGameplayModifierInfo());
	FGameplayModifierInfo& ModifierInfo = GE->Modifiers[Index];

	ModifierInfo.ModifierMagnitude = FScalableFloat(DebuffDamage);
	ModifierInfo.ModifierOp = EGameplayModOp::Additive;
	ModifierInfo.Attribute = UTDAttributeSet::GetIncomingDamageAttribute();

	// GameplayEffectSpec 동적 생성.
	FGameplayEffectSpec* MutableSpec = new FGameplayEffectSpec(GE, GEContextHandle, 1.f);
	if (MutableSpec)
	{
		FTDGameplayEffectContext* TDGEContext = static_cast<FTDGameplayEffectContext*>(MutableSpec->GetContext().Get());
		TSharedPtr<FGameplayTag> DebuffDamageType = MakeShareable(new FGameplayTag(DamageType));
		TDGEContext->SetDamageType(DebuffDamageType);

		Props.TargetASC->ApplyGameplayEffectSpecToSelf(*MutableSpec); // 상대방에 디버프 적용!
	}
}

// 경험치 획득 후 레벨업, AttributePoint, SkillPoint 업데이트
void UTDAttributeSet::ApplyIncomingExp(const FEffectProperties& Props)
{
	const float LocalIncomingExp = GetIncomingExp();
	SetIncomingExp(0.f);

	IICombat* CombatInterface = Cast<IICombat>(Props.SourceCharacter);
	IIPlayer* PlayerInterface = Cast<IIPlayer>(Props.SourceCharacter);
	if (PlayerInterface && CombatInterface)
	{
		const int32 CurrentPlayerLevel = CombatInterface->GetPlayerLevel(); // 현재 레벨
		const int32 CurrentExp = PlayerInterface->GetExp(); // 현재 경험치

		const int32 NewPlayerLevel = PlayerInterface->FindLevelForExp(CurrentExp + LocalIncomingExp); // 경험치 추가 후 새로운 레벨
		const int32 AmountOfLevelUps = NewPlayerLevel - CurrentPlayerLevel; // 레벨 변동값

		if (AmountOfLevelUps > 0)
		{
			PlayerInterface->AddToPlayerLevel(AmountOfLevelUps); // 레벨업

			int32 AttributePointsReward = 0;
			int32 SkillPointsReward = 0;

			for (int32 i = 0; i < AmountOfLevelUps; i++) // 레벨업한만큼 Attribute, Skill 포인트 준비. 
			{
				AttributePointsReward += PlayerInterface->GetAttributePointsReward(CurrentPlayerLevel + i);
				SkillPointsReward += PlayerInterface->GetSkillPointsReward(CurrentPlayerLevel + i);
			}

			PlayerInterface->AddToAttributePoints(AttributePointsReward); // Attribute Point 추가
			PlayerInterface->AddToSkillPoints(SkillPointsReward);		  // Skill Point 추가

			// bMakeMaxHealth, bMakeMaxMana = true면 PostAttributeChange()에서 체력과 마나 최대로 설정
			bMakeMaxHealth = true;
			bMakeMaxMana = true;

			PlayerInterface->LevelUpCPP(); // 레벨업
		}

		PlayerInterface->AddToExpCPP(LocalIncomingExp);
	}
}

void UTDAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const // 이펙트 속성 설정하기
{
	// Source = causer of the effect, Target = target of the effect (owner of this AttributeSet)

	Props.EffectContextHandle = Data.EffectSpec.GetContext();
	Props.SourceASC = Props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

	if (IsValid(Props.SourceASC) && Props.SourceASC->AbilityActorInfo.IsValid() && Props.SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.SourceAvatarActor = Props.SourceASC->AbilityActorInfo->AvatarActor.Get();
		Props.SourceController = Props.SourceASC->AbilityActorInfo->PlayerController.Get();
		if (Props.SourceController == nullptr && Props.SourceAvatarActor != nullptr)
		{
			if (const APawn* Pawn = Cast<APawn>(Props.SourceAvatarActor))
			{
				Props.SourceController = Pawn->GetController();
			}
		}
		if (Props.SourceController)
		{
			Props.SourceCharacter = Cast<ACharacter>(Props.SourceController->GetPawn());
		}
	}

	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		Props.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		Props.TargetCharacter = Cast<ACharacter>(Props.TargetAvatarActor);
		Props.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetAvatarActor);
	}
}

void UTDAttributeSet::ShowFloatingText(const FEffectProperties& Props, float Damage, bool bBlockedHit, bool bCriticalHit) const
{
	if (Props.SourceCharacter != Props.TargetCharacter)
	{
		// SouceCharacter가 플레이어. TargetCharcter(= damage causer)가 몬스터.
		if (ATDPlayerController* PC = Cast<ATDPlayerController>(Props.SourceCharacter->Controller))
		{
			PC->ShowDamageNumber(Damage, Props.TargetCharacter, bBlockedHit, bCriticalHit);
			return; // 리턴.
		}
		// SouceCharacter가 몬스터. TargetCharacter(= damage causer 공격한 사람)가 플레이어.
		if (ATDPlayerController* PC = Cast<ATDPlayerController>(Props.TargetCharacter->Controller))
		{
			PC->ShowDamageNumber(Damage, Props.TargetCharacter, bBlockedHit, bCriticalHit);
		}
	}
}

void UTDAttributeSet::SendExpEvent(const FEffectProperties& Props) // 자신을 죽인 캐릭터에 경험치 보내기
{
	IICombat* CombatInterface = Cast<IICombat>(Props.TargetCharacter);
	if (CombatInterface)
	{
		const int32 TargetLevel = CombatInterface->GetPlayerLevel();
		//const ECharacterClass TargetClass = IICombat::Execute_GetCharacterClass(Props.TargetCharacter);
		const ECharacterClass TargetClass = CombatInterface->GetCharacterClassCPP();
		const int32 ExpReward = UTDAbilitySystemBPLibrary::GetExpRewardForClassAndLevel(Props.TargetCharacter, TargetClass, TargetLevel);

		const FTDGameplayTags& GameplayTags = FTDGameplayTags::GetTDGameplayTags();
		FGameplayEventData Payload;
		Payload.EventTag = GameplayTags.Attributes_Meta_IncomingExp;
		Payload.EventMagnitude = ExpReward;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Props.SourceCharacter, GameplayTags.Attributes_Meta_IncomingExp, Payload);
	}
}

void UTDAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTDAttributeSet, Strength, OldStrength);
}

void UTDAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTDAttributeSet, Intelligence, OldIntelligence);
}

void UTDAttributeSet::OnRep_Resilience(const FGameplayAttributeData& OldResilience) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTDAttributeSet, Resilience, OldResilience);
}

void UTDAttributeSet::OnRep_Vigor(const FGameplayAttributeData& OldVigor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTDAttributeSet, Vigor, OldVigor);
}

void UTDAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTDAttributeSet, Armor, OldArmor);
}

void UTDAttributeSet::OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTDAttributeSet, ArmorPenetration, OldArmorPenetration);
}

void UTDAttributeSet::OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTDAttributeSet, BlockChance, OldBlockChance);
}

void UTDAttributeSet::OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTDAttributeSet, CriticalHitChance, OldCriticalHitChance);
}

void UTDAttributeSet::OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTDAttributeSet, CriticalHitDamage, OldCriticalHitDamage);
}

void UTDAttributeSet::OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTDAttributeSet, CriticalHitResistance, OldCriticalHitResistance);
}

void UTDAttributeSet::OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTDAttributeSet, HealthRegeneration, OldHealthRegeneration);
}

void UTDAttributeSet::OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTDAttributeSet, ManaRegeneration, OldManaRegeneration);
}

void UTDAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTDAttributeSet, MaxHealth, OldMaxHealth);
}

void UTDAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTDAttributeSet, MaxMana, OldMaxMana);
}

void UTDAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTDAttributeSet, Health, OldHealth);
}

void UTDAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTDAttributeSet, Mana, OldMana);
}

void UTDAttributeSet::OnRep_Soul(const FGameplayAttributeData& OldSoul) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTDAttributeSet, Soul, OldSoul);
}

void UTDAttributeSet::OnRep_PhysicalResistance(const FGameplayAttributeData& OldPhysicalResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTDAttributeSet, PhysicalResistance, OldPhysicalResistance);
}

void UTDAttributeSet::OnRep_FireResistance(const FGameplayAttributeData& OldFireResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTDAttributeSet, FireResistance, OldFireResistance);
}

void UTDAttributeSet::OnRep_IceResistance(const FGameplayAttributeData& OldIceResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTDAttributeSet, IceResistance, OldIceResistance);
}

void UTDAttributeSet::OnRep_ElectronicResistance(const FGameplayAttributeData& OldElectronicResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTDAttributeSet, ElectronicResistance, OldElectronicResistance);
}

void UTDAttributeSet::OnRep_UltimateResistance(const FGameplayAttributeData& OldUltimateResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTDAttributeSet, UltimateResistance, OldUltimateResistance);
}
