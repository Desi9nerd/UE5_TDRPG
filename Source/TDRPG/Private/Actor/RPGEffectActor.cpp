#include "Actor/RPGEffectActor.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GAS/RPGAttributeSet.h"
#include "Components/SphereComponent.h"

ARPGEffectActor::ARPGEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(Mesh);

	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	Sphere->SetupAttachment(GetRootComponent());
}

void ARPGEffectActor::BeginPlay()
{
	Super::BeginPlay();

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ARPGEffectActor::OnOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &ARPGEffectActor::EndOverlap);
}

void ARPGEffectActor::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//TODO: 지금은 const_cast 사용. 추후에 Gameplay Effect 적용방법으로 수정하자.
	TWeakObjectPtr<IAbilitySystemInterface> ASCInterface = Cast<IAbilitySystemInterface>(OtherActor);
	if (ASCInterface.IsValid())
	{
		const URPGAttributeSet* RPGAttributeSet = Cast<URPGAttributeSet>(ASCInterface->GetAbilitySystemComponent()->GetAttributeSet(URPGAttributeSet::StaticClass()));

		URPGAttributeSet* MutableRPGAttributeSet = const_cast<URPGAttributeSet*>(RPGAttributeSet);
		MutableRPGAttributeSet->SetHealth(RPGAttributeSet->GetHealth() + 25.f); // HP를 +25

		Destroy();
	}
}

void ARPGEffectActor::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}
