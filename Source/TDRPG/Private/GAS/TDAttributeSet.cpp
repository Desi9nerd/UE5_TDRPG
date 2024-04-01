#include "GAS/TDAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameFramework/Character.h"
#include "GameplayEffectExtension.h"
#include "GameplayTags/TDGameplayTags.h"
#include "GAS/TDAbilitySystemBPLibrary.h"
#include "Interface/ICombat.h"
#include "Kismet/GameplayStatics.h"
#include "Player/TDPlayerController.h"

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
	TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Fireball, GetFireballResistanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Meteor, GetMeteorResistanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Melee, GetMeleeResistanceAttribute);
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
	DOREPLIFETIME_CONDITION_NOTIFY(UTDAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTDAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTDAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTDAttributeSet, FireballResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTDAttributeSet, MeteorResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTDAttributeSet, MeleeResistance, COND_None, REPNOTIFY_Always);
}

void UTDAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	// PreAttributeChange함수는 Attribute의 변수 변경 전에 관찰하여 범위를 설정해둔 값이 범위를 벗어났을시 적용되지 않게 한다. Clamp 용도로만 사용하는걸 권장한다.
	Super::PreAttributeChange(Attribute, NewValue);

	//** 수치 Clamp 범위 주기 (attribute 변경 전에 clamp 적용)
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
		//UE_LOG(LogTemp, Warning, TEXT("Health: %f"), NewValue);
	}
	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
	}
}

void UTDAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FEffectProperties Props;
	SetEffectProperties(Data, Props); // 이펙트 속성 설정하기

	//** 수치 Clamp 범위 주기 (attribute 변경 후에 clamp 적용)
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
		UE_LOG(LogTemp, Warning, TEXT("Changed Health on %s, Health: %f"), *Props.TargetAvatarActor->GetName(), GetHealth());
	}
	if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.f, GetMaxMana()));
	}

	//** Damage 처리
	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
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
					CombatInterface->Die(); // 캐릭터 사망 처리
				}
			}
			else // 체력 0 초과
			{
				FGameplayTagContainer TagContainer;
				TagContainer.AddTag(FTDGameplayTags::GetTDGameplayTags().Effect_HitReact);
				Props.TargetASC->TryActivateAbilitiesByTag(TagContainer); // Effect_HitReact 태그가 있는 모든 GameplayAbility들을 활성화
			}

			const bool bBlock = UTDAbilitySystemBPLibrary::IsBlockedHit(Props.EffectContextHandle);
			const bool bCriticalHit = UTDAbilitySystemBPLibrary::IsCriticalHit(Props.EffectContextHandle);
			ShowFloatingText(Props, LocalIncomingDamage, bBlock, bCriticalHit); // 데미지 숫자 띄우기. FEffectProperties, 데미지, Block, CriticalHit 정보 전달
		}
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
		if (ATDPlayerController* PC = Cast<ATDPlayerController>(Props.SourceCharacter->Controller))
		{
			PC->ShowDamageNumber(Damage, Props.TargetCharacter, bBlockedHit, bCriticalHit);
		}
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

void UTDAttributeSet::OnRep_FireballResistance(const FGameplayAttributeData& OldFireballResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTDAttributeSet, FireballResistance, OldFireballResistance);
}

void UTDAttributeSet::OnRep_MeteorResistance(const FGameplayAttributeData& OldMeteorResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTDAttributeSet, MeteorResistance, OldMeteorResistance);
}

void UTDAttributeSet::OnRep_MeleeResistance(const FGameplayAttributeData& OldMeleeResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTDAttributeSet, MeleeResistance, OldMeleeResistance);
}
