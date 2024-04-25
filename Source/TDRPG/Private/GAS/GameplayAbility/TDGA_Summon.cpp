#include "GAS/GameplayAbility/TDGA_Summon.h"
#include "Kismet/KismetSystemLibrary.h"

TArray<FVector> UTDGA_Summon::GetSpawnLocations() // 명령한 캐릭터 주변으로 미니언들이 스폰되는 위치들
{
	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	const float DeltaSpread = SpawnSpread / NumMinions;


	const FVector LeftOfSpread = Forward.RotateAngleAxis(-SpawnSpread / 2.f, FVector::UpVector); // Z-axis 기준으로 회전
	TArray<FVector> SpawnLocations; // 미니언들 스폰 위치들

	for (int32 i = 0; i < NumMinions; i++)
	{
		FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector); // DeltaSpread만큼 회전
		FVector ChosenSpawnLocation = Location + Direction * FMath::FRandRange(MinSpawnDistance, MaxSpawnDistance); // 앞뒤 범위 내 랜덤지점

		//DrawDebugSphere(GetWorld(), ChosenSpawnLocation, 18.f, 12, FColor::Cyan, false, 3.f);
		//UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), Location, Location + Direction * MaxSpawnDistance, 4.f, FLinearColor::Green, 3.f);
		//DrawDebugSphere(GetWorld(), Location + Direction * MinSpawnDistance, 5.f, 12, FColor::Red, false, 3.f);
		//DrawDebugSphere(GetWorld(), Location + Direction * MaxSpawnDistance, 5.f, 12, FColor::Red, false, 3.f);

		//** 바닥에서 스폰되게 하기********************
		FHitResult Hit;
		GetWorld()->LineTraceSingleByChannel(Hit, ChosenSpawnLocation + FVector(0.f, 0.f, 400.f), ChosenSpawnLocation - FVector(0.f, 0.f, 400.f), ECC_Visibility);
		if (Hit.bBlockingHit)
		{
			ChosenSpawnLocation = Hit.ImpactPoint;
		}
		//*******************************************

		SpawnLocations.Add(ChosenSpawnLocation);
	}

	return SpawnLocations;
}

TSubclassOf<APawn> UTDGA_Summon::GetRandomMinionClass() // MinionClasses에 담긴 미니언 종류 중 랜덤으로 하나 선택
{
	const int32 Selection = FMath::RandRange(0, MinionClasses.Num() - 1);

	return MinionClasses[Selection];
}
