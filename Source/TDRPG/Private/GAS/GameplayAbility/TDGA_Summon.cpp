#include "GAS/GameplayAbility/TDGA_Summon.h"
#include "Kismet/KismetSystemLibrary.h"

TArray<FVector> UTDGA_Summon::GetSpawnLocations() // ����� ĳ���� �ֺ����� �̴Ͼ���� �����Ǵ� ��ġ��
{
	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	const float DeltaSpread = SpawnSpread / NumMinions;


	const FVector LeftOfSpread = Forward.RotateAngleAxis(-SpawnSpread / 2.f, FVector::UpVector); // Z-axis �������� ȸ��
	TArray<FVector> SpawnLocations; // �̴Ͼ�� ���� ��ġ��

	for (int32 i = 0; i < NumMinions; i++)
	{
		FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector); // DeltaSpread��ŭ ȸ��
		FVector ChosenSpawnLocation = Location + Direction * FMath::FRandRange(MinSpawnDistance, MaxSpawnDistance); // �յ� ���� �� ��������

		//DrawDebugSphere(GetWorld(), ChosenSpawnLocation, 18.f, 12, FColor::Cyan, false, 3.f);
		//UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), Location, Location + Direction * MaxSpawnDistance, 4.f, FLinearColor::Green, 3.f);
		//DrawDebugSphere(GetWorld(), Location + Direction * MinSpawnDistance, 5.f, 12, FColor::Red, false, 3.f);
		//DrawDebugSphere(GetWorld(), Location + Direction * MaxSpawnDistance, 5.f, 12, FColor::Red, false, 3.f);

		//** �ٴڿ��� �����ǰ� �ϱ�********************
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

TSubclassOf<APawn> UTDGA_Summon::GetRandomMinionClass() // MinionClasses�� ��� �̴Ͼ� ���� �� �������� �ϳ� ����
{
	const int32 Selection = FMath::RandRange(0, MinionClasses.Num() - 1);

	return MinionClasses[Selection];
}
