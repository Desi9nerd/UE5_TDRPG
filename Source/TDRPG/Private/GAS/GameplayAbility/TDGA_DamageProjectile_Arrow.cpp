#include "GAS/GameplayAbility/TDGA_DamageProjectile_Arrow.h"
#include "Interface/ICombat.h"
#include "Kismet/KismetSystemLibrary.h"

FString UTDGA_DamageProjectile_Arrow::GetDescription(int32 Level)
{
	return Super::GetDescription(Level);
}

FString UTDGA_DamageProjectile_Arrow::GetNextAbilityLevelDescription(int32 AbilityLevel)
{
	return Super::GetNextAbilityLevelDescription(AbilityLevel);
}

void UTDGA_DamageProjectile_Arrow::SpawnProjectiles(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, bool bOverridePitch, float PitchOverride, AActor* HomingTarget)
{
	if (false == GetAvatarActorFromActorInfo()->HasAuthority()) return;

	IICombat* CombatInterface = Cast<IICombat>(GetAvatarActorFromActorInfo());

	const FVector SocketLocation = CombatInterface->GetCombatSocketLocationCPP(SocketTag);
	FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
	if (bOverridePitch) Rotation.Pitch = PitchOverride;

	const FVector Forward = Rotation.Vector();
	const FVector LeftOfSpread = Forward.RotateAngleAxis(-ArrowSpread / 2.f, FVector::UpVector);
	const FVector RightOfSpread = Forward.RotateAngleAxis(ArrowSpread / 2.f, FVector::UpVector);
	
	if (NumProjectiles > 1) // Arrow 2개 이상. ArrowSpread = FMath::Min(MaxNumArrows, GetAbilityLevel());
	{
		const float DeltaSpread = ArrowSpread / (NumProjectiles - 1);
		for (int32 i = 0; i < NumProjectiles; i++)
		{
			const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
			const FVector Start = SocketLocation + FVector(0, 0, 5);
			UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), Start, Start + Direction * 75.f, 1, FLinearColor::Yellow, 60, 1);
		}
	}
	else // Arrow 1개
	{
		const FVector Start = SocketLocation + FVector(0, 0, 5);
		UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), Start, Start + Forward * 75.f, 1, FLinearColor::Yellow, 60, 1);
	}

	UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), SocketLocation, SocketLocation + Forward * 100.f, 1, FLinearColor::White, 120, 1);
	UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), SocketLocation, SocketLocation + LeftOfSpread * 100.f, 1, FLinearColor::Gray, 120, 1);
	UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), SocketLocation, SocketLocation + RightOfSpread * 100.f, 1, FLinearColor::Gray, 120, 1);
}
