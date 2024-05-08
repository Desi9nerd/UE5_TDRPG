


#include "Actor/TDAura.h"

// Sets default values
ATDAura::ATDAura()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATDAura::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATDAura::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

