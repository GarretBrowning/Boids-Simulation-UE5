#include "Flock.h"
#include "Boid.h"
#include "Waypoint.h"
#include "Kismet/GameplayStatics.h" // Needed for grabbing all of the Boids Actors within the level.

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

	if (UWorld* World = GetWorld())
	{
		// Setting the Min/Max Bounds for the Boids to spawn within:
		const FVector BoxMin = GetActorLocation() - SpawnBoxExtent;
		const FVector BoxMax = GetActorLocation() + SpawnBoxExtent;

		// Spawning a Set NumberOfBoids with a Random Transform within the given area:
		for (size_t i = 0; i < NumberOfBoids; i++)
		{
			float RandomScale = FMath::RandRange(0.5f, 1.0f); // Random scale value between 0.5f - 1.0f.

			// Creating a Random Transform to spawn each Boid with:
			FTransform RandomTransform
			{
				FRotator{FMath::RandRange(-360.0f, 360.0f), FMath::RandRange(-360.0f, 360.0f), FMath::RandRange(-360.0f, 360.0f)}, // Random Rotation
				FVector{FMath::RandPointInBox(FBox(BoxMin, BoxMax))}, // Random Location (Within SpawnBox)
				FVector{RandomScale, RandomScale, RandomScale} // Random Scale (Uniform)
			};

			World->SpawnActor<AActor>(BoidClass, RandomTransform); // Requires that World (our pointer to the World object) is not const.
		}

		// Creating a temporary TArray to store the Boids within (stored as AActor objects).
		TArray<AActor*> TempCollection;

		// Iterating through the valid list of Boid actors within the current level and adding them to the temporary collection:
		UGameplayStatics::GetAllActorsOfClass(World, ABoid::StaticClass(), TempCollection);

		// Casting each AActor object within the temporary collection to an ABoid object and filling our own personal TArray (myBoids):
		for (AActor* Actor : TempCollection)
		{
			// Casting the Actors to class Boid:
			myBoids.Add(static_cast<ABoid*>(Actor));
		}

		// Setting the detection radius, minimum speed, and maximum speed of each Boid:
		for (auto Boid : myBoids)
		{
			Boid->SetDetectionRadius(IndividualDetectionRadius);
			Boid->SetMinimumSpeed(MinimumSpeed);
			Boid->SetMaximumSpeed(MaximumSpeed);
		}

		// Emptying the temporary array (will use it again):
		TempCollection.Empty();

		// Iterating through the valid list of Waypoint actors within the current level and adding them to the temporary collection:
		UGameplayStatics::GetAllActorsOfClass(World, AWaypoint::StaticClass(), TempCollection);

		// Casting each AActor object within the temporary collection to an AWaypoint object and filling our own personal TArray (myWaypoints):
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

	// Apply Movement to each Boid in the flock every frame:
	if (bEnableMovement)
	{
		for (auto Boid : myBoids)
		{
			Boid->ApplyMovement(DeltaTime, CalculateAlignment(Boid), CalculateCohesion(Boid), CalculateSeparation(Boid), CalculateWaypointAttraction(Boid));
		}
	}

	// If Debugging is Enabled, Shows the Specific Debug Values based upon the given flags:
	if (DEBUG_MODE_ENABLED)
	{
		for (auto Boid : myBoids)
		{
			if (bShowDetectionRadius)
			{
				Boid->DebugDrawDetectionRadius();
			}

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
		}
	}
}

// Called to bind functionality to input
void AFlock::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

// Calculates the Alignment vector for the given Boid:
FVector AFlock::CalculateAlignment(ABoid* aBoid)
{
	TArray<FVector> BoidDirections;
	FVector DirectionSum(0.f);
	FVector AverageDirection(0.f);
	FVector Alignment(0.f);

	// Iterate through all boids and add their forward vectors to an array:
	for (auto Boid : myBoids)
	{
		// Checking to see that we do not evaluate against ourselves.
		// Checking if the current Boid we are comparing to is within our detection radius.
		if (Boid != aBoid && aBoid->GetDistanceTo(Boid) < aBoid->GetDetectionRadius()) 
		{
			BoidDirections.Add(Boid->GetActorForwardVector());
		}
	}

	// Finding the average forward direction from the given array of Boid forward vectors:
	if (BoidDirections.Num() > 0) // Ensuring that there is at least 1 Boid that we have detected (otherwise we will not be influenced).
	{
		// Summing all of the forward vectors of each detected Boid:
		for (int32 VecIdx = 0; VecIdx < BoidDirections.Num(); VecIdx++)
		{
			DirectionSum += BoidDirections[VecIdx];
		}

		// Calculating the average forward vector of all detected Boids and setting that as the Average Direction:
		AverageDirection = DirectionSum / ((float)BoidDirections.Num());

		// 1. Getting the normal of the Average Forward Direction.
		// 2. Multiplying the Average Forward Direction by the AlignmentMultiplier scalar value.
		Alignment = AverageDirection.GetSafeNormal() * AlignmentMultiplier;
	}

	return Alignment;
}


// Calculates the Cohesion vector for the given Boid:
FVector AFlock::CalculateCohesion(ABoid* aBoid)
{
	TArray<FVector> BoidLocations;
	FVector LocationSum = FVector::ZeroVector; // Alternative way of creating a (0,0,0) vector.
	FVector AverageLocation(0.f);
	FVector Cohesion(0.f);

	// Iterate through all boids and add their locations to an array:
	for (auto Boid : myBoids)
	{
		// Checking to see that we do not evaluate against ourselves.
		// Checking if the current Boid we are comparing to is within our detection radius.
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

// Calculates the Separation vector for the given Boid:
FVector AFlock::CalculateSeparation(ABoid* aBoid)
{
	TArray<FVector> BoidVectors;
	TArray<float> BoidDistances;
	FVector SeparationSum(0.f);
	FVector AverageSeparation(0.f);
	FVector Separation(0.f);

	// Iterate through the list of Boids:
	for (auto Boid : myBoids)
	{
		// Checking to see that we do not evaluate against ourselves.
		// Checking if the current Boid we are comparing to is within our detection radius.
		if (aBoid != Boid && aBoid->GetDistanceTo(Boid) < aBoid->GetDetectionRadius())
		{
			// Finding the distance between the current Boid and target Boid:
			float Distance = aBoid->GetDistanceTo(Boid); // Magnitude/Length

			// Finding the direction of vector pointing from current Boid and target Boid:
			FVector Direction = (aBoid->GetActorLocation() - Boid->GetActorLocation()).GetSafeNormal();

			// Finding the scaling proximity multiplier of the vector based upon how close the current Boid is to the target Boid:
			float ProximityMultiplier = 1.0f - (Distance / Boid->GetDetectionRadius());

			// Adding the calculated vector into the collection of total Boid vectors:
			BoidVectors.Add(Direction * ProximityMultiplier);
		}
	}

	// Finding the average separation value from the given array of Boid vectors:
	if (BoidVectors.Num() > 0) // Ensuring that there is at least 1 Boid that we have detected (otherwise we will not be influenced).
	{
		// Summing the values of each vector within the BoidVectors collection:
		for (int32 VecIdx = 0; VecIdx < BoidVectors.Num(); VecIdx++)
		{
			SeparationSum += BoidVectors[VecIdx];
		}

		// Calculating the Average Separation value:
		AverageSeparation = SeparationSum / ((float)BoidVectors.Num());

		Separation = AverageSeparation * SeparationMultiplier; // Unsure on whether or not to normalize this.
		//Separation = AverageSeparation.GetSafeNormal() * SeparationMultiplier;
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

	// Error checking to ensure we have at least 1 Waypoint active within the scene:
	if (myWaypoints.Num() > 0)
	{
		for (auto Waypoint : myWaypoints)
		{
			// Finding the distance from the given Boid to the Waypoint:
			float DistanceToWaypoint = aBoid->GetDistanceTo(Waypoint);

			// Only calculate the attraction force to steer us if we are within the Waypoint's attraction radius:
			if (DistanceToWaypoint < Waypoint->GetAttractionRadius())
			{

				// Find direction of vector pointing from Boid to target Waypoint:
				FVector Direction = (Waypoint->GetActorLocation() - aBoid->GetActorLocation()).GetSafeNormal();

				// The farther we are away from the Waypoint, the stronger the attraction:
				float ProximityMultiplier = DistanceToWaypoint / Waypoint->GetAttractionRadius();

				// Add the attraction force to the collection of Waypoint vectors:
				WaypointVectors.Add(Direction * ProximityMultiplier);
			}
		}
	}

	if (WaypointVectors.Num() > 0) // Ensuring that there is at least 1 Waypoint that we have detected (otherwise we will not be influenced).
	{
		// Summing the values of all detected Waypoint vectors:
		for (int32 VecIdx = 0; VecIdx < WaypointVectors.Num(); VecIdx++)
		{
			AttractionSum += WaypointVectors[VecIdx];
		}

		// Calculating the average attraction force:
		AverageAttraction = AttractionSum / ((float)WaypointVectors.Num());

		WaypointAttraction = AverageAttraction * WaypointAttractionMultiplier;
	}
	
	return WaypointAttraction;
}

