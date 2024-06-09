#include "GAS/TDAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GAS/GameplayAbility/TDGA.h"
#include "GameplayTags/TDGameplayTags.h"
#include "GAS/TDAbilitySystemBPLibrary.h"
#include "GAS/Data/TDDA_Ability.h"
#include "Interface/IPlayer.h"
#include "SaveGame/TDSaveGame_Load.h"

void UTDAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UTDAbilitySystemComponent::ClientEffectApplied); // Delegate에 바인딩. Delegate가 서버에서 broadcast되면 서버에서 콜 되어 클라이언트에서 실행된다.
}

// SaveGame에 저장된 Abilities들을 추가할 때 불려진다.
void UTDAbilitySystemComponent::AddCharacterAbilitiesFromSaveData(UTDSaveGame_Load* SaveData)
{
	for (const FSavedAbility& Data : SaveData->SavedAbilities)
	{
		const TSubclassOf<UGameplayAbility> LoadedAbilityClass = Data.GameplayAbility;

		FGameplayAbilitySpec LoadedAbilitySpec = FGameplayAbilitySpec(LoadedAbilityClass, Data.AbilityLevel);

		LoadedAbilitySpec.DynamicAbilityTags.AddTag(Data.AbilitySlot);
		LoadedAbilitySpec.DynamicAbilityTags.AddTag(Data.AbilityStatus);

		if (Data.AbilityType == FTDGameplayTags::GetTDGameplayTags().Abilities_Type_Active) // Active
		{
			GiveAbility(LoadedAbilitySpec); // Abilities 할당.
		}
		else if (Data.AbilityType == FTDGameplayTags::GetTDGameplayTags().Abilities_Type_Passive) // Passive
		{
			if (Data.AbilityStatus.MatchesTagExact(FTDGameplayTags::GetTDGameplayTags().Abilities_Status_Equipped))
			{
				GiveAbilityAndActivateOnce(LoadedAbilitySpec);
				//MulticastActivatePassiveEffect(Data.AbilityTag, true);
			}
			else
			{
				GiveAbility(LoadedAbilitySpec); // Abilities 할당.
			}
		}
	}

	bStartGivenASC = true;
	GivenASCDelegate.Broadcast();
}

// AddCharacterAbilities()함수는 서버에서만 불려진다.
// GivenASCDelegate을 클라이언트들에게도 Broadcast하기 위해서 아래의 OnRep_ActivateAbilities()함수를 사용하였다.
void UTDAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	// 에디터에서 할당한 GameplayAbility를 모두 등록
	for (const TSubclassOf<UGameplayAbility> AbilityClass : StartupAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		const TObjectPtr<UTDGA> TDAbility = Cast<UTDGA>(AbilitySpec.Ability);
		if (IsValid(TDAbility))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(TDAbility->StartupInputTag); // InputTag 등록
			AbilitySpec.DynamicAbilityTags.AddTag(FTDGameplayTags::GetTDGameplayTags().Abilities_Status_Equipped); // Abilities_Status_Eligible 태그 등록
			GiveAbility(AbilitySpec);
			//MarkAbilitySpecDirty(AbilitySpec);
		}
	}

	bStartGivenASC = true;
	GivenASCDelegate.Broadcast();
}

void UTDAbilitySystemComponent::AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupPassiveAbilities)
{
	// 에디터에서 할당한 GameplayAbility를 모두 등록
	for (const TSubclassOf<UGameplayAbility> AbilityClass : StartupPassiveAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		AbilitySpec.DynamicAbilityTags.AddTag(FTDGameplayTags::GetTDGameplayTags().Abilities_Status_Equipped); // Abilities 장착 상태로 설정.
		GiveAbilityAndActivateOnce(AbilitySpec); // Activate
	}
}

void UTDAbilitySystemComponent::InputTagPressed(const FGameplayTag& InputTag)
{
	if (false == InputTag.IsValid()) return;

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputReleased(AbilitySpec);
			
			if (AbilitySpec.IsActive())
			{
				InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, AbilitySpec.Handle, AbilitySpec.ActivationInfo.GetActivationPredictionKey());
			}
		}
	}
}

void UTDAbilitySystemComponent::InputTagReleased(const FGameplayTag& InputTag)
{
	if (false == InputTag.IsValid()) return;

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputReleased(AbilitySpec);

			if (AbilitySpec.IsActive()) // 계속 누르고 있는 상태였으면
			{
				InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, AbilitySpec.Handle, AbilitySpec.ActivationInfo.GetActivationPredictionKey());
			}
		}
	}
}

void UTDAbilitySystemComponent::InputTagHeld(const FGameplayTag& InputTag)
{
	if (false == InputTag.IsValid()) return;

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec); // 내장함수.
			if (false == AbilitySpec.IsActive())
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
}

void UTDAbilitySystemComponent::ForEachAbility(const FForEachAbility& Delegate)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (false == Delegate.ExecuteIfBound(AbilitySpec))
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to execute delegate in %hs"), __FUNCTION__);
		}
	}
}

void UTDAbilitySystemComponent::EnhanceAttribute(const FGameplayTag& AttributeTag)
{
	IIPlayer* PlayerInterface = Cast<IIPlayer>(GetAvatarActor());
	if (PlayerInterface)
	{
		if (PlayerInterface->GetAttributePoints() > 0)
		{
			ServerEnhanceAttribute(AttributeTag); // Server에 알려줌.
		}
	}
}

void UTDAbilitySystemComponent::ServerEnhanceAttribute_Implementation(const FGameplayTag& AttributeTag)
{
	FGameplayEventData Payload;
	Payload.EventTag = AttributeTag;
	Payload.EventMagnitude = 1.f;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetAvatarActor(), AttributeTag, Payload);

	IIPlayer* PlayerInterface = Cast<IIPlayer>(GetAvatarActor());
	if (PlayerInterface)
	{
		PlayerInterface->AddToAttributePoints(-1);
	}
}

// 어빌리티 업데이트. 새로운 어빌리티 추가할게 있는지 확인하고 추가하기.
void UTDAbilitySystemComponent::UpdateAbilityStatuses(int32 PlayerLevel)
{
	UTDDA_Ability* TDDA_Ability = UTDAbilitySystemBPLibrary::GetTDDA_Ability(GetAvatarActor());
	for (const FDA_Ability& DA_Ability_Iter : TDDA_Ability->DA_AbilityInfo)
	{
		if (false == DA_Ability_Iter.AbilityTag.IsValid()) continue; // AbilityTag가 없다면 continue.
		if (PlayerLevel < DA_Ability_Iter.LevelRequirement) continue; // PlayerLevel이 레벨조건 보다 낮다면 continue.

		if (GetSpecFromAbilityTag(DA_Ability_Iter.AbilityTag) == nullptr)
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(DA_Ability_Iter.Ability, 1);
			AbilitySpec.DynamicAbilityTags.AddTag((FTDGameplayTags::GetTDGameplayTags().Abilities_Status_Eligible)); // 런타임에 태그 추가.
			GiveAbility(AbilitySpec); // 어빌리티 부여.
			MarkAbilitySpecDirty(AbilitySpec);

			ClientUpdateAbilityStatus(DA_Ability_Iter.AbilityTag, FTDGameplayTags::GetTDGameplayTags().Abilities_Status_Eligible, 1);
		}
	}
}

bool UTDAbilitySystemComponent::GetDescriptionsByAbilityTag(const FGameplayTag& AbilityTag, FString& OutDescription, FString& OutNextLevelDescription)
{
	//** Activatable Abilities에서 Ability를 찾는 경우
	const FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag);
	if (AbilitySpec)
	{
		if (UTDGA* TDGA = Cast<UTDGA>(AbilitySpec->Ability))
		{
			OutDescription = TDGA->GetDescription(AbilitySpec->Level);
			OutNextLevelDescription = TDGA->GetNextAbilityLevelDescription(AbilitySpec->Level + 1);
			return true;
		}
	}

	//** Activatable Abilities에서 Ability를 찾지 못한 경우
	const UTDDA_Ability* TDDA_Ability = UTDAbilitySystemBPLibrary::GetTDDA_Ability(GetAvatarActor());
	if (false == AbilityTag.IsValid() || AbilityTag.MatchesTagExact(FTDGameplayTags::GetTDGameplayTags().Abilities_None))
	{
		OutDescription = FString();
	}
	else
	{
		OutDescription = UTDGA::GetLockedDescription(TDDA_Ability->FindDA_AbilityForTag(AbilityTag).LevelRequirement);
	}
	OutNextLevelDescription = FString();

	return false;
}

bool UTDAbilitySystemComponent::SkillMenuAbilityHasSlot(FGameplayAbilitySpec* Spec, const FGameplayTag& SlotTag)
{
	for (FGameplayTag TagIter : Spec->DynamicAbilityTags) // DynamicAbilityTags들 중에
	{
		if (TagIter.MatchesTag(SlotTag)) return true; // SlotTag가 있으면 true 리턴.
	}
	return false;
}

// 매개변수로 들어온 Spec의 Ability가 가지고 있는 SlotInputTag 없애기. 
void UTDAbilitySystemComponent::SkillMenuClearSlot(FGameplayAbilitySpec* Spec)
{
	const FGameplayTag SlotInputTag = GetInputTagFromSpec(*Spec);
	Spec->DynamicAbilityTags.RemoveTag(SlotInputTag);
	MarkAbilitySpecDirty(*Spec);
}

// ActivatableAbilities를 모두 검사하여 DynamicAbilityTags들 중 SlotTag 모두 없애기.
void UTDAbilitySystemComponent::SkillMenuClearAbilitiesOfSlot(const FGameplayTag& SlotTag)
{
	FScopedAbilityListLock ActiveScopedLock(*this);

	for (FGameplayAbilitySpec& SpecIter : GetActivatableAbilities()) // ActivatableAbilities들 중
	{
		if (SkillMenuAbilityHasSlot(&SpecIter, SlotTag)) // DynamicAbilityTags들 중 SlotTag가 있으면
		{
			SkillMenuClearSlot(&SpecIter); // 해당 Slot(Input)Tag 없애기.
		}
	}
}

// SkillPoint를 소모하여 스킬획득.
// Server RPC로 클라이언트들에서 호출된 후 정보 수정 후 Client RPC로 서버에 알린다.
void UTDAbilitySystemComponent::ServerSpendSkillPoint_Implementation(const FGameplayTag& AbilityTag)
{
	FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag);
	if (AbilitySpec)
	{
		//* SkillPoint 소모.
		IIPlayer* PlayerInterface = Cast<IIPlayer>(GetAvatarActor());
		if (PlayerInterface)
		{
			PlayerInterface->AddToSkillPoints(-1); // SkillPoint 1 소모.
		}

		//* AbilitySpec 정보 수정(StatusTag를 업데이트 또는 Ability->Level 업데이트). 
		const FTDGameplayTags TDGameplayTags = FTDGameplayTags::GetTDGameplayTags();
		FGameplayTag StatusTag = GetStatusTagFromSpec(*AbilitySpec);
		if (StatusTag.MatchesTagExact(TDGameplayTags.Abilities_Status_Eligible))
		{
			// Status를 Unlock으로 변경.
			AbilitySpec->DynamicAbilityTags.RemoveTag(TDGameplayTags.Abilities_Status_Eligible);
			AbilitySpec->DynamicAbilityTags.AddTag(TDGameplayTags.Abilities_Status_Unlocked);
			StatusTag = TDGameplayTags.Abilities_Status_Unlocked;
		}
		else if (StatusTag.MatchesTagExact(TDGameplayTags.Abilities_Status_Equipped) || StatusTag.MatchesTagExact(TDGameplayTags.Abilities_Status_Unlocked)) // 장착중 또는 해금된 상태
		{
			AbilitySpec->Level += 1; // 주의: PlayerLevel 아님.
		}

		ClientUpdateAbilityStatus(AbilityTag, StatusTag, AbilitySpec->Level); // Client RPC. 변경된 사항을 서버에 알린다.
		MarkAbilitySpecDirty(*AbilitySpec);
	}
}

FGameplayTag UTDAbilitySystemComponent::GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	if (AbilitySpec.Ability)
	{
		for (FGameplayTag Tag : AbilitySpec.Ability.Get()->AbilityTags)
		{
			if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities")))) // "Abilities"라는 이름이 있다면
			{
				return Tag;
			}
		}
	}
	return FGameplayTag();
}

FGameplayTag UTDAbilitySystemComponent::GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for (FGameplayTag Tag : AbilitySpec.DynamicAbilityTags)
	{
		if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("InputTag"))))
		{
			return Tag;
		}
	}
	return FGameplayTag();
}

FGameplayTag UTDAbilitySystemComponent::GetStatusTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for (FGameplayTag StatusTag : AbilitySpec.DynamicAbilityTags)
	{
		if (StatusTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities.Status"))))
		{
			return StatusTag;
		}
	}
	return FGameplayTag();
}

// Server RPC
void UTDAbilitySystemComponent::ServerEquipAbility_Implementation(const FGameplayTag& AbilityTag, const FGameplayTag& SlotTag)
{
	if (FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		const FGameplayTag& PrevSlotTag = GetInputTagFromSpec(*AbilitySpec);
		const FGameplayTag& StatusTag = GetStatusTagFromSpec(*AbilitySpec);

		// Status가 Equipped 또는 Unlocked 상태라면
		if (StatusTag == FTDGameplayTags::GetTDGameplayTags().Abilities_Status_Equipped ||
			StatusTag == FTDGameplayTags::GetTDGameplayTags().Abilities_Status_Unlocked)
		{
			// TODO : ServerEquipAbility_Implementation 수정하기

			SkillMenuClearAbilitiesOfSlot(SlotTag); // SlotTag 모두 없애기
			SkillMenuClearSlot(AbilitySpec);		// 
			AbilitySpec->DynamicAbilityTags.AddTag(SlotTag); // 해당 SlotTag를 담기.

			// Status:  Unlock -> Equipped 변경.
			if (StatusTag.MatchesTagExact(FTDGameplayTags::GetTDGameplayTags().Abilities_Status_Unlocked))
			{
				AbilitySpec->DynamicAbilityTags.RemoveTag(FTDGameplayTags::GetTDGameplayTags().Abilities_Status_Unlocked);
				AbilitySpec->DynamicAbilityTags.AddTag(FTDGameplayTags::GetTDGameplayTags().Abilities_Status_Equipped);
			}
			MarkAbilitySpecDirty(*AbilitySpec);
		}

		//AbilitySpec->DynamicAbilityTags.RemoveTag(GetStatusFromSpec(*AbilitySpec));
		//AbilitySpec->DynamicAbilityTags.AddTag(GameplayTags.Abilities_Status_Equipped);

		// Client들에게도 실행.
		EquipAbility(AbilityTag, FTDGameplayTags::GetTDGameplayTags().Abilities_Status_Equipped, SlotTag, PrevSlotTag);
	}
}

// Client들에서 실행. Equipped Ability가 변경되었음을 Broadcast로 알려줌.
void UTDAbilitySystemComponent::EquipAbility(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, const FGameplayTag& SlotTag, const FGameplayTag& PreviousSlotTag)
{
	EquippedAbilityDelegate.Broadcast(AbilityTag, StatusTag, SlotTag, PreviousSlotTag);
}

FGameplayAbilitySpec* UTDAbilitySystemComponent::GetSpecFromAbilityTag(const FGameplayTag& AbilityTag)
{
	/** Used to stop us from removing abilities from an ability system component while we're iterating through the abilities */
	FScopedAbilityListLock ActiveScopeLoc(*this);

	for (FGameplayAbilitySpec& AbilitySpecIter : GetActivatableAbilities())
	{
		for (FGameplayTag TagIter : AbilitySpecIter.Ability.Get()->AbilityTags)
		{
			if (TagIter.MatchesTag(AbilityTag))
			{
				// ActivatableAbilities 중 AbilityTag와 일치하는 태그를 가지고 있는게 있다면 AbilitySpec를 리턴.
				return &AbilitySpecIter;
			}
		}
	}

	return nullptr;
}

FGameplayTag UTDAbilitySystemComponent::GetStatusTagFromAbilityTag(const FGameplayTag& AbilityTag)
{
	if (const FGameplayAbilitySpec* Spec = GetSpecFromAbilityTag(AbilityTag))
	{
		return GetStatusTagFromSpec(*Spec);
	}
	return FGameplayTag();
}

FGameplayTag UTDAbilitySystemComponent::GetInputTagFromAbilityTag(const FGameplayTag& AbilityTag) // Input, Slot 동일
{
	if (const FGameplayAbilitySpec* Spec = GetSpecFromAbilityTag(AbilityTag))
	{
		return GetInputTagFromSpec(*Spec);
	}
	return FGameplayTag();
}

void UTDAbilitySystemComponent::ClientUpdateAbilityStatus_Implementation(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 AbilityLevel)
{
	AbilityStatusChangedDelegate.Broadcast(AbilityTag, StatusTag, AbilityLevel);
}

// 상위 클래스인 AbilitySystemComponent에 ActivateAbilities 값이 변경될 때마다 호출되는 OnRep_ActivateAbilities 함수를 재정의하여 사용.
// GivenASCDelegate을 클라이언트들에게도 Broadcast한다.
void UTDAbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();

	if (false == bStartGivenASC)
	{
		bStartGivenASC = true;
		GivenASCDelegate.Broadcast();
	}
}

// AbilitySystemComponent.h 107줄
// DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnGameplayEffectAppliedDelegate, UAbilitySystemComponent*, const FGameplayEffectSpec&, FActiveGameplayEffectHandle);에 사용하기 위해 3개의 매개변수 사용
// FOnGameplayEffectAppliedDelegate OnGameplayEffectAppliedDelegateToSelf;
// 정리하면, 이 ASC에 GameplayEffect가 적용되면 아래의 함수가 콜백되어 실행된다.
void UTDAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent,
	const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);

	EffectAssetTagsDelegate.Broadcast(TagContainer);
}
