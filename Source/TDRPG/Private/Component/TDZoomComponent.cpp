#include "Component/TDZoomComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"

UTDZoomComponent::UTDZoomComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTDZoomComponent::BeginPlay()
{
	Super::BeginPlay();

	SpringArm = Cast<USpringArmComponent>(GetOwner()->GetComponentByClass(USpringArmComponent::StaticClass()));
	checkf(SpringArm, TEXT("No SpringArm. Check UTDZoomComponent::BeginPlay()"));

	CurrentValue = SpringArm->TargetArmLength;
}

void UTDZoomComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	checkf(SpringArm, TEXT("No SpringArm. Check UTDZoomComponent::TickComponent()"));

	if (UKismetMathLibrary::NearlyEqual_FloatFloat(SpringArm->TargetArmLength, CurrentValue)) return;

	SpringArm->TargetArmLength = UKismetMathLibrary::FInterpTo(SpringArm->TargetArmLength, CurrentValue, DeltaTime, InterpSpeed);
}

void UTDZoomComponent::SetZoomValue(float InValue)
{
	CurrentValue -= (Speed * InValue);
	CurrentValue = FMath::Clamp(CurrentValue, Range.X, Range.Y);
}
