#include "UI/WidgetController/TDWidgetControllerOverlay.h"
#include "GAS/TDAbilitySystemComponent.h"
#include "GAS/TDAttributeSet.h"
#include "GAS/Data/TDDA_Ability.h"

void UTDWidgetControllerOverlay::BroadcastInitialValues()
{
	const UTDAttributeSet* TDAttributeSet = CastChecked<UTDAttributeSet>(AttributeSet);

	OnHealthChanged.Broadcast(TDAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(TDAttributeSet->GetMaxHealth());
	OnManaChanged.Broadcast(TDAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(TDAttributeSet->GetMaxMana());
	OnSoulChanged.Broadcast(TDAttributeSet->GetSoul());
}

void UTDWidgetControllerOverlay::BindCallbacksToDependencies() // TDAttributeSet�� �����Ϳ� �ݹ��Լ� ���ε�
{
	const UTDAttributeSet* TDAttributeSet = CastChecked<UTDAttributeSet>(AttributeSet);

	//** Health, MaxHealth�� ����ɶ� ���� �Ʒ��Լ�(HealthChanged, MaxHealthChanged)�� callback��
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		TDAttributeSet->GetHealthAttribute()).AddUObject(this, &UTDWidgetControllerOverlay::HealthChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		TDAttributeSet->GetMaxHealthAttribute()).AddUObject(this, &UTDWidgetControllerOverlay::MaxHealthChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		TDAttributeSet->GetManaAttribute()).AddUObject(this, &UTDWidgetControllerOverlay::ManaChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		TDAttributeSet->GetMaxManaAttribute()).AddUObject(this, &UTDWidgetControllerOverlay::MaxManaChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		TDAttributeSet->GetSoulAttribute()).AddUObject(this, &UTDWidgetControllerOverlay::SoulChanged);


	/*Cast<UTDAbilitySystemComponent>(AbilitySystemComponent)->EffectAssetTags.AddLambda(
		[this](const FGameplayTagContainer& AssetTags)
		{
			for (const FGameplayTag& Tag : AssetTags)
			{
				const FString Msg = FString::Printf(TEXT("GE Tag: %s"), *Tag.ToString());
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, Msg);

				FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
			}
		}
	);*/ // ��: �����Լ� ����, �Ʒ�: �Լ��� ������ �� ���� 
	UTDAbilitySystemComponent* TDASC = Cast<UTDAbilitySystemComponent>(AbilitySystemComponent);
	if (IsValid(TDASC))
	{
		if (TDASC->bStartGivenASC) // AbilitySystemComponent �����Ͱ� ����Ǿ� �ִٸ�
		{
			OnInitializeStartGivenASC(TDASC); 
		}
		else // AbilitySystemComponent �����Ͱ� ������ �� �Ǿ� �ִٸ�
		{
			TDASC->GivenASCDelegate.AddUObject(this, &UTDWidgetControllerOverlay::OnInitializeStartGivenASC);		
		}

		TDASC->EffectAssetTags.AddUObject(this, &UTDWidgetControllerOverlay::ReadDataTableRowByTag);
	}
}

void UTDWidgetControllerOverlay::ReadDataTableRowByTag(const FGameplayTagContainer& AssetTags)
{
	for (const FGameplayTag& Tag : AssetTags)
	{
		FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
		if (Tag.MatchesTag(MessageTag)) // MatchesTag�� "Message"���ڸ� �����ϰ� �ִ��� Ȯ��
		{
			const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
			MessageWidgetRowDelegate.Broadcast(*Row); // Delegate Broadcast
		}
	}
}

void UTDWidgetControllerOverlay::OnInitializeStartGivenASC(UTDAbilitySystemComponent* TDASC)
{
	if (false == TDASC->bStartGivenASC) return; // ����ڵ�. �̹� ����Ǿ��ٸ� �ٽ� �������� �ʰ� ����.

	// ������ TDASC ������ ��ε�ĳ����.
	// BroadcastDelegate ���� �� ���ٷ� ���ε�.
	FForEachAbility BroadcastDelegate; 
	BroadcastDelegate.BindLambda([this, TDASC](const FGameplayAbilitySpec& AbilitySpec)
		{
			//TODO: Given Ability Spec�� ���� AbilityTag �ʿ�
			FDA_Ability DA_AbilityInfo = TDDA_Ability->FindDA_AbilityForTag(TDASC->GetAbilityTagFromSpec(AbilitySpec));
			DA_AbilityInfo.InputTag = TDASC->GetInputTagFromSpec(AbilitySpec); // � InputTag�� � AbilitySpec�� ��Ī�Ǵ��� �˱�����
			DA_AbilityInfoDelegate.Broadcast(DA_AbilityInfo);
		});

	TDASC->ForEachAbility(BroadcastDelegate); // ��������Ʈ ������
}

void UTDWidgetControllerOverlay::HealthChanged(const FOnAttributeChangeData& Data) const
{
	OnHealthChanged.Broadcast(Data.NewValue);
}

void UTDWidgetControllerOverlay::MaxHealthChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxHealthChanged.Broadcast(Data.NewValue);
}

void UTDWidgetControllerOverlay::ManaChanged(const FOnAttributeChangeData& Data) const
{
	OnManaChanged.Broadcast(Data.NewValue);
}

void UTDWidgetControllerOverlay::MaxManaChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxManaChanged.Broadcast(Data.NewValue);
}

void UTDWidgetControllerOverlay::SoulChanged(const FOnAttributeChangeData& Data) const
{
	OnSoulChanged.Broadcast(Data.NewValue);
}
