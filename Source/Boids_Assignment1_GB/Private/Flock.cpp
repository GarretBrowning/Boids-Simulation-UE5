// Fill out your copyright notice in the Description page of Project Settings.

#include "Flock.h"
#include "Boid.h"
#include "Waypoint.h"
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

	// Need to now spawn the Boids randomly within a given spawn zone:
	// Will try and do this within the flock class here:

	int32 spawnNumber = 20;


	// NEED TO RANDOMIZE LOCATION
	// NEED TO RANDOMIZE ROTATION BETWEEN -360 -360 ON EACH AXIS
	// Scale should be the same (unless we also want this to be random as well (perhaps 0.5-1 range).
	FTransform A
	{
	FRotator{0.0f, 0.0f, 0.0f},                 // Rotation
	FVector{1.0f, 2.0f, 3.0f},  // Translation
	FVector{2.0f, 2.0f, 2.0f}   // Scale
	};


	for (size_t i = 0; i < spawnNumber; i++)
	{
		// This method will not work by doing the usual world pointer protection, I'm not sure why...
	




		//GetWorld()->SpawnActor<AActor>()
	}



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

		// Setting the detection radius of each Boid:
		for (auto Boid : myBoids)
		{
			Boid->SetDetectionRadius(IndividualDetectionRadius);
		}
	}

	// Iterating through the valid list of Waypoint actors within the current level and adding them to the Waypoints collection:
	if (const UWorld* World = GetWorld())
	{
		TArray<AActor*> TempCollection;
		UGameplayStatics::GetAllActorsOfClass(World, AWaypoint::StaticClass(), TempCollection);

		for (AActor* Actor : TempCollection)
		{
			// Casting the Actors to class Waypoint:
			myWaypoints.Add(static_cast<AWaypoint*>(Actor));
		}
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
			if (bShowAlignmentDebugLines)
			{
				Boid->DebugAlignment(CalculateAlignment(Boid));
			}

			if (bShowCohesionDebugLines)
			{
				Boid->DebugCohesion(CalculateCohesion(Boid));
			}

			if (bShowSeparationDebugLines)
			{
				Boid->DebugSeparation(CalculateSeparation(Boid));
			}

			if (bShowWaypointDebugLines)
			{
				Boid->DebugWaypointAttraction(CalculateWaypointAttraction(Boid));
			}

			if (bShowDetectionRadius)
			{
				Boid->DebugDrawDetectionRadius();
			}
		}

		//for (auto Waypoint : myWaypoints)
		//{
		//	if (UWorld* world = GetWorld())
		//	{
		//		GEngine->AddOnScreenDebugMessage(FMath::Rand(), world->GetDeltaSeconds(), FColor::Orange, FString::Printf(TEXT("%s"), *Waypoint->GetName()));
		//		//GEngine->AddOnScreenDebugMessage(FMath::Rand(), world->GetDeltaSeconds(), FColor::Purple, FString::Printf(TEXT("%s's Acceleration: %s"), *this->GetName(), *myAcceleration.ToString()));
		//	}
		//}
		// Apply Movement to each Boid in the flock every frame:
		if (bEnableMovement)
		{
			for (auto Boid : myBoids)
			{
				//ACTUAL MOVEMENT CODE ---->
				Boid->ApplyMovement(DeltaTime, CalculateAlignment(Boid), CalculateCohesion(Boid), CalculateSeparation(Boid));
				Boid->ApplyMovement(DeltaTime, CalculateAlignment(Boid), CalculateCohesion(Boid), CalculateSeparation(Boid), CalculateWaypointAttraction(Boid));

				// Only calculating separation rn:
				//Boid->ApplyMovement(DeltaTime, FVector::ZeroVector, CalculateCohesion(Boid), CalculateSeparation(Boid));
				//Boid->ApplyMovement(DeltaTime, FVector::ZeroVector, CalculateCohesion(Boid), FVector::ZeroVector);
				//Boid->ApplyMovement(DeltaTime, CalculateAlignment(Boid), FVector::ZeroVector, FVector::ZeroVector);
				//Boid->ApplyMovement(DeltaTime, FVector::ZeroVector, FVector::ZeroVector, CalculateSeparation(Boid));

				//Boid->ApplyMovement(DeltaTime, FVector::ZeroVector, FVector::ZeroVector, FVector::ZeroVector);

			}
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
		// Checking if we are not evaluating against ourselves & seeing if the Boid we are comparing to is within our detection radius.
		if (Boid != aBoid && aBoid->GetDistanceTo(Boid) < aBoid->GetDetectionRadius()) 
		//if (Boid!= aBoid)
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
	FVector LocationSum = FVector::ZeroVector; // Fun alternative way of creating a (0,0,0) vector.
	FVector AverageLocation(0.f);
	FVector Cohesion(0.f);

	// Iterate through all boids and add their locations to an array:
	for (auto Boid : myBoids)
	{
		// Excludes adding itself or any Boid that is not within its detection radius:
		if(Boid != aBoid && aBoid->GetDistanceTo(Boid) < aBoid->GetDetectionRadius())
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
		
		// 1. Calculating a vector with direction pointing towards the average boid location.
		// 2. Getting the normal of this vector.
		// 3. Multiplying the normal by the CohesionMultiplier scalar value.
		Cohesion = (AverageLocation - aBoid->GetActorLocation()).GetSafeNormal() * CohesionMultiplier;
	}

	return Cohesion; 
}

FVector AFlock::CalculateSeparation(ABoid* aBoid)
{
	TArray<FVector> BoidVectors;
	TArray<float> BoidDistances;
	FVector SeparationSum(0.f);
	FVector AverageSeparation(0.f);

	FVector Separation(0.f);

	// iterate through list of boids to find vector in direction facing away from those boids
	for (auto Boid : myBoids)
	{
		// #Todo: Should add nullptr protection

		// Excludes adding itself or any Boid that is not within its detection radius:
		if (aBoid != Boid && aBoid->GetDistanceTo(Boid) < aBoid->GetDetectionRadius())
		{
			// Find distance between the current Boid and target Boid:
			float Distance = aBoid->GetDistanceTo(Boid); // Magnitude/Length

			float Distance2 = (aBoid->GetActorLocation() - Boid->GetActorLocation()).Size();

			// 300.0f 

			/// Radius 300.0f
			///	length / 300.0f
			///	if (1.0f - length / 300.0f > 0)
			///	1.0f - length / 300.0f == ProxV
			///	(Direction * ProxV) * ScalarMultiplierValue aka Strength

			// Find direction of vector pointing from current Boid and target Boid:
			FVector Direction = (aBoid->GetActorLocation() - Boid->GetActorLocation()).GetSafeNormal(); // Unsure of whether to normalize or not...

			// Direction * ProximityMultiplier
			// 1.0 - 

			//FVector Direction = aBoid->GetActorLocation() - Boid->GetActorLocation(); // Or WHEN to normalize...

			// Add vector to the array of total calculated separation vectors (based upon distance to other Boid):
			//BoidVectors.Add(Direction / (Distance * Distance));

			// Alternative attempt at trying to inversely calculate vector: (Doesn't work)
			//float ProximityMultiplier = 1.0f - (Direction.Size() / Distance); // (1 - 1/1)
			float ProximityMultiplier = 1.0f - (Distance / Boid->GetDetectionRadius()); // THIS SHOULD BE THE ONE!

			BoidVectors.Add(Direction * ProximityMultiplier);
		}
	}

	if (BoidVectors.Num() > 0)
	{
		for (int32 VecIdx = 0; VecIdx < BoidVectors.Num(); VecIdx++)
		{
			SeparationSum += BoidVectors[VecIdx];
		}

		AverageSeparation = SeparationSum / ((float)BoidVectors.Num());

		/*if (const UWorld* world = GetWorld())
		{
			GEngine->AddOnScreenDebugMessage(FMath::Rand(), world->GetDeltaSeconds(), FColor::Black, FString::Printf(TEXT("Average Separation: %s"), *AverageSeparation.ToString()));
		}*/

		//Separation = AverageSeparation.GetSafeNormal() * SeparationMultiplier; // //Correct one
		Separation = AverageSeparation * SeparationMultiplier; // * Distance Scalar 
	}

	return Separation;
}

FVector AFlock::CalculateWaypointAttraction(ABoid* aBoid)
{
	TArray<FVector> WaypointVectors;
	TArray<float> WaypointDistances;
	FVector AttractionSum(0.f);
	FVector AverageAttraction(0.f);

	FVector WaypointAttraction(0.f);

	// Error checking to ensure we ahve at least 1 waypoint active in the scene
	if (myWaypoints.Num() > 0)
	{
		for (auto Waypoint : myWaypoints)
		{
			float DistanceToWaypoint = aBoid->GetDistanceTo(Waypoint); // Magnitude/Length

			// Only calculate steering force if we are within the waypoint attraction radius
			if (DistanceToWaypoint < Waypoint->GetAttractionRadius())
			{

				// Find direction of vector pointing from Boid to target waypoint:
				FVector Direction = (Waypoint->GetActorLocation() - aBoid->GetActorLocation()).GetSafeNormal(); // Unsure of whether to normalize or not...


				float ProximityMultiplier = DistanceToWaypoint / Waypoint->GetAttractionRadius(); // THIS SHOULD BE THE ONE!

				WaypointVectors.Add(Direction * ProximityMultiplier);
			}
		}
	}

	if (WaypointVectors.Num() > 0)
	{
		for (int32 VecIdx = 0; VecIdx < WaypointVectors.Num(); VecIdx++)
		{
			AttractionSum += WaypointVectors[VecIdx];
		}

		AverageAttraction = AttractionSum / ((float)WaypointVectors.Num());

		/*if (const UWorld* world = GetWorld())
		{
			GEngine->AddOnScreenDebugMessage(FMath::Rand(), world->GetDeltaSeconds(), FColor::Black, FString::Printf(TEXT("Average Separation: %s"), *AverageSeparation.ToString()));
		}*/

		//Separation = AverageSeparation.GetSafeNormal() * SeparationMultiplier; // //Correct one
		WaypointAttraction = AverageAttraction * WaypointAttractionMultiplier; // * Distance Scalar 
	}
	
	return WaypointAttraction;
}

