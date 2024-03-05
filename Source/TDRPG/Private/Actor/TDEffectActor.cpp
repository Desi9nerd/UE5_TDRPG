#include "Actor/TDEffectActor.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GAS/TDAttributeSet.h"
#include "Components/SphereComponent.h"

ATDEffectActor::ATDEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(Mesh);

	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	Sphere->SetupAttachment(GetRootComponent());
}

void ATDEffectActor::BeginPlay()
{
	Super::BeginPlay();

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ATDEffectActor::OnOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &ATDEffectActor::EndOverlap);
}

void ATDEffectActor::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//TODO: ������ const_cast ���. ���Ŀ� Gameplay Effect ���������� ��������.
	IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(OtherActor);
	if (ASCInterface)
	{
		const UTDAttributeSet* TDAttributeSet = Cast<UTDAttributeSet>(ASCInterface->GetAbilitySystemComponent()->GetAttributeSet(UTDAttributeSet::StaticClass()));

		UTDAttributeSet* MutableTDAttributeSet = const_cast<UTDAttributeSet*>(TDAttributeSet);
		MutableTDAttributeSet->SetHealth(TDAttributeSet->GetHealth() + 25.f); // HP�� +25

		Destroy();
	}
}

void ATDEffectActor::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}
