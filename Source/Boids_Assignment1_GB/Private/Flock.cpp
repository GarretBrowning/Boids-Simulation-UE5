// Fill out your copyright notice in the Description page of Project Settings.


#include "Flock.h"

#include "Boid.h"
#include "Kismet/GameplayStatics.h" // Needed for grabbing all ofo the Boids within the current game level.
#include "UObject/UnrealTypePrivate.h"

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
		TArray<AActor*> TempCollection;
		UGameplayStatics::GetAllActorsOfClass(World, ABoid::StaticClass(), TempCollection);

		for (AActor* Actor : TempCollection)
		{
			// Casting the Actors to class Boid:
			myBoids.Add(static_cast<ABoid*>(Actor));
		}

		//GEngine->AddOnScreenDebugMessage(FMath::Rand(), 5.0f, FColor::Red, FString::FromInt(myBoids.Num()));
	}
}

// Called every frame
void AFlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (DEBUG_MODE_ENABLED)
	{
		for (auto Boid : myBoids)
		{
			// Boid->DebugCohesion(CalculateCohesion(Boid));
			// Boid->DebugAlignment(CalculateAlignment(Boid));
			Boid->DebugSeparation(CalculateSeparation(Boid));
		}
	}

}

// Called to bind functionality to input
void AFlock::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

FVector AFlock::CalculateAlignment(ABoid* aBoid)
{
	// Find the forward vector of each Boid and get the average:
	TArray<FVector> BoidDirections;
	FVector DirectionSum(0.f);
	FVector AverageDirection(0.f);

	FVector Alignment(0.f);

	// Iterate through all boids and add their forward vectors to an array:
	for (auto Boid : myBoids)
	{
		if (Boid != aBoid)
		{
			BoidDirections.Add(Boid->GetActorForwardVector());
		}
	}

	// Finding the average forward direction from the given array of Boid forward vectors:
	if (BoidDirections.Num() > 0)
	{
		for (int32 VecIdx = 0; VecIdx < BoidDirections.Num(); VecIdx++)
		{
			DirectionSum += BoidDirections[VecIdx];
		}

		AverageDirection = DirectionSum / ((float)BoidDirections.Num());


		Alignment = AverageDirection.GetSafeNormal() * AlignmentMultiplier;
	}

	return Alignment;
}

FVector AFlock::CalculateCohesion(ABoid* aBoid)
{
	TArray<FVector> BoidLocations;
	FVector LocationSum(0.f);
	FVector AverageLocation(0.f);

	FVector Cohesion(0.f);

	// Iterate through all boids and add their locations to an array:
	for (auto Boid : myBoids)
	{
		if(Boid != aBoid)
		{
			BoidLocations.Add(Boid->GetActorLocation());
		}
	}

	// Finding the average location from the given array of Boid locations:
	if (BoidLocations.Num() > 0)
	{
		for (int32 VecIdx = 0; VecIdx < BoidLocations.Num(); VecIdx++)
		{
			LocationSum += BoidLocations[VecIdx];
		}

		AverageLocation = LocationSum / ((float)BoidLocations.Num());
	}

	// 1. Calculating a vector with direction pointing towards the average boid location.
	// 2. Getting the normal of this vector.
	// 3. Multiplying the normal by the CohesionMultiplier scalar value.
	Cohesion = (AverageLocation - aBoid->GetActorLocation()).GetSafeNormal() * CohesionMultiplier;

	return Cohesion; 
}

FVector AFlock::CalculateSeparation(ABoid* aBoid)
{
	TArray<FVector> BoidVectors;
	TArray<float> BoidDistances;
	FVector LocationSum(0.f);
	FVector AverageLocation(0.f);

	FVector Separation(0.f);

	// iterate through list of boids to find vector in direction facing away from those boids
	for (auto Boid : myBoids)
	{
		if (aBoid != Boid)
		{
			float Distance = aBoid->GetDistanceTo(Boid);
			// Find distance between the current Boid and target Boid:
			//BoidDistances.Add()

			// (1/distance*distance) * normalized vector

			FVector Direction = (aBoid->GetActorLocation() - Boid->GetActorLocation()).GetSafeNormal();
			



			BoidVectors.Add(aBoid->GetActorLocation() - Boid->GetActorLocation());
			//Separation = (aBoid->GetActorLocation() - Boid->GetActorLocation()).GetSafeNormal() * SeparationMultiplier;
		}
	}

	if (BoidVectors.Num() > 0)
	{
		for (int32 VecIdx = 0; VecIdx < BoidVectors.Num(); VecIdx++)
		{
			LocationSum += BoidVectors[VecIdx];
		}

		AverageLocation = LocationSum / ((float)BoidVectors.Num());

		Separation = AverageLocation.GetSafeNormal() * SeparationMultiplier; // * Distance Scalar
	}

	

	return Separation;
}

