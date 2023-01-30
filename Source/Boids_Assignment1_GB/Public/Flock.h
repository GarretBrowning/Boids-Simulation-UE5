// This is the main class responsible for driving the logic behind all Boid's within the level.
// The "flock" calculates all of the movement vectors required for applying movement to each individual Boid.
// ** The multiplier values for each movement rule are not final (I haven't found the optimal ratio yet). **

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Flock.generated.h"

// Global Constexpr Variable Used As A Debugging Flag:
constexpr auto DEBUG_MODE_ENABLED = 1; // Recommended conversion away from an #if defined Macro (according to ReSharper).

// Forward Declarations for Boid and Waypoint classes:
class ABoid;
class AWaypoint;

UCLASS()
class BOIDS_ASSIGNMENT1_GB_API AFlock : public APawn
{
	GENERATED_BODY()

	// DEFAULT STARTING VALUES //
	
	// Flag for whether or not we'd like to enable movement (mainly used for debugging purposes).
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Boids | Defaults", meta = (AllowPrivateAccess = "true"))
	bool bEnableMovement{ false };

	// The detection radius of each individual Boid.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Boids | Defaults", meta = (AllowPrivateAccess = "true", ClampMin = "50.0", ClampMax = "2000.0"))
	float IndividualDetectionRadius{ 0.f };

	// The minimum speed of each individual Boid.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Boids | Defaults", meta = (AllowPrivateAccess = "true", ClampMin = "1.0", ClampMax = "150.0"))
	float MinimumSpeed{ 30.0f };

	// The maximum speed of each individual Boid.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Boids | Defaults", meta = (AllowPrivateAccess = "true", ClampMin = "1.0", ClampMax = "150.0"))
	float MaximumSpeed{ 70.0f };


	// SPAWNING PROPERTIES //

	// The outer box extent used to calculate where the Boids will be randomly spawned.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Boids | Spawning", meta = (AllowPrivateAccess = "true"))
	FVector SpawnBoxExtent{800.0f, 800.0f, 800.0f};

	// The number of Boids that will be spawned.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Boids | Spawning", meta = (AllowPrivateAccess = "true", ClampMin = "1", ClampMax = "200"))
	int NumberOfBoids{ 5 };

	// The Blueprint class to spawn.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Boids | Spawning", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ABoid> BoidClass;

	// Collection of all Boids within the level.
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	TArray<ABoid*> myBoids;

	// Collection of all Waypoints within the level.
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	TArray<AWaypoint*> myWaypoints;

	
	// MULTIPLIER VALUES //

	// Value to multiply the Alignment steering rule.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Boids | Multiplier Value", meta = (AllowPrivateAccess = "true"))
	float AlignmentMultiplier{ 1.0 };

	// Value to multiply the Cohesion steering rule.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Boids | Multiplier Value", meta = (AllowPrivateAccess = "true"))
	float CohesionMultiplier{ 1.0 };

	// Value to multiply the Separation steering rule.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Boids | Multiplier Value", meta = (AllowPrivateAccess = "true"))
	float SeparationMultiplier{ 1.0 };

	// Value to multiply the Waypoint Attraction.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Boids | Multiplier Value", meta = (AllowPrivateAccess = "true"))
	float WaypointAttractionMultiplier{ 1.0 };


	// DEBUGGING PROPERTIES //

	// Shows a visual debug sphere of each Boid's detection radius.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Boids | Debugging", meta = (AllowPrivateAccess = "true"))
	bool bShowDetectionRadius{ false };

	// Shows the values of each calculated Alignment vector along with a visual debug line.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Boids | Debugging", meta = (AllowPrivateAccess = "true"))
	bool bShowAlignmentDebugLines{ false };

	// Shows the values of each calculated Cohesion vector along with a visual debug line.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Boids | Debugging", meta = (AllowPrivateAccess = "true"))
	bool bShowCohesionDebugLines{ false };

	// Shows the values of each calculated Separation vector along with a visual debug line.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Boids | Debugging", meta = (AllowPrivateAccess = "true"))
	bool bShowSeparationDebugLines{ false };

	// Shows the values of each calculated Waypoint Attraction vector along with a visual debug line.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Boids | Debugging", meta = (AllowPrivateAccess = "true"))
	bool bShowWaypointDebugLines{ false };

public:
	// Sets default values for this pawn's properties
	AFlock();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	// CLASSIC BOID RULES //

	// Calculating Alignment Vector:
	FVector CalculateAlignment(ABoid* aBoid);

	// Calculating Cohesion Vector:
	FVector CalculateCohesion(ABoid* aBoid);

	// Calculating Separation Vector:
	FVector CalculateSeparation(ABoid* aBoid);


	// WAYPOINT RULE //

	// Calculating Waypoint Attraction Vector:
	FVector CalculateWaypointAttraction(ABoid* aBoid);
};
