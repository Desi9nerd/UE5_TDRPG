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

void UTDWidgetControllerOverlay::BindCallbacksToDependencies() // TDAttributeSet의 데이터와 콜백함수 바인딩
{
	const UTDAttributeSet* TDAttributeSet = CastChecked<UTDAttributeSet>(AttributeSet);

	//** Health, MaxHealth가 변경될때 마다 아래함수(HealthChanged, MaxHealthChanged)가 callback됨
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
	);*/ // 위: 람다함수 버젼, 아래: 함수를 밖으로 뺀 버젼 
	UTDAbilitySystemComponent* TDASC = Cast<UTDAbilitySystemComponent>(AbilitySystemComponent);
	if (IsValid(TDASC))
	{
		if (TDASC->bStartGivenASC) // AbilitySystemComponent 데이터가 적용되어 있다면
		{
			OnInitializeStartGivenASC(TDASC); 
		}
		else // AbilitySystemComponent 데이터가 적용이 안 되어 있다면
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
		if (Tag.MatchesTag(MessageTag)) // MatchesTag로 "Message"글자를 포함하고 있는지 확인
		{
			const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
			MessageWidgetRowDelegate.Broadcast(*Row); // Delegate Broadcast
		}
	}
}

void UTDWidgetControllerOverlay::OnInitializeStartGivenASC(UTDAbilitySystemComponent* TDASC)
{
	if (false == TDASC->bStartGivenASC) return; // 방어코드. 이미 적용되었다면 다시 적용하지 않고 리턴.

	// 위젯에 TDASC 데이터 브로드캐스팅.
	// BroadcastDelegate 생성 후 람다로 바인딩.
	FForEachAbility BroadcastDelegate; 
	BroadcastDelegate.BindLambda([this, TDASC](const FGameplayAbilitySpec& AbilitySpec)
		{
			//TODO: Given Ability Spec을 위한 AbilityTag 필요
			FDA_Ability DA_AbilityInfo = TDDA_Ability->FindDA_AbilityForTag(TDASC->GetAbilityTagFromSpec(AbilitySpec));
			DA_AbilityInfo.InputTag = TDASC->GetInputTagFromSpec(AbilitySpec); // 어떤 InputTag와 어떤 AbilitySpec이 매칭되는지 알기위해
			DA_AbilityInfoDelegate.Broadcast(DA_AbilityInfo);
		});

	TDASC->ForEachAbility(BroadcastDelegate); // 델리게이트 보내기
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
