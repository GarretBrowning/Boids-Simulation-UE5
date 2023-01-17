// Fill out your copyright notice in the Description page of Project Settings.


#include "Flock.h"

#include "Boid.h"
#include "Kismet/GameplayStatics.h" // Needed for grabbing all ofo the Boids within the current game level.

// Sets default values
AFlock::AFlock()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFlock::BeginPlay()
{
	Super::BeginPlay();

	// Iterating through the valid list of Boid actors within the current level and adding them to the Boids collection:
	if (const UWorld* World = GetWorld())
	{
		UGameplayStatics::GetAllActorsOfClass(World, ABoid::StaticClass(), myBoids);
	}
	GEngine->AddOnScreenDebugMessage(FMath::Rand(), 5.0f, FColor::Red, FString::FromInt(myBoids.Num()));
	
}

// Called every frame
void AFlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFlock::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

//FVector AFlock::CalculateCohesion(ABoid* aBoid)
//{
//	//TArray< AllActorLocations[];
//	for (auto Boid : myBoids)
//	{
//		//if(Cast<ABoid>(Boid) != aBoid)
//		//{
//			
//		//}
//	}
//
//	return
//}

