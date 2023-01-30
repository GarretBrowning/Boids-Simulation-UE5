// The base Boid class used for representing each individual flock member.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Boid.generated.h"

UCLASS()
class BOIDS_ASSIGNMENT1_GB_API ABoid : public APawn
{
	GENERATED_BODY()

	// Root Component
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent* myRoot;

	// Name of the Root Component
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FName myRootComponentName = "Root";

	// Boid Visual Representation (Cone)
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* myBoidBody;

	// Name of the Boid's Body (StaticMeshComponent / Cone)
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FName myBoidBodyName = "Boid Body";

	// Detection Radius
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Boid | Movement", meta = (AllowPrivateAccess = "true"))
	float myDetectionRadius{ 100.0 };

	// Minimum Speed
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "Boid | Movement", meta = (AllowPrivateAccess = "true"))
	float myMinSpeed{ 30.0f };

	// Maximum Speed
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "Boid | Movement", meta = (AllowPrivateAccess = "true"))
	float myMaxSpeed{ 70.0f };

	// Accumulated/Current Velocity
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Boid | Movement", meta = (AllowPrivateAccess = "true"))
	FVector myVelocity {0.f};

	// Acceleration
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Boid | Movement", meta = (AllowPrivateAccess = "true"))
	FVector myAcceleration {0.f};

	// Simple multiplier used to help draw the debug lines
	float myDebugLineMultiplier{ 10.0f };

public:
	// Sets default values for this pawn's properties
	ABoid();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Called to apply movement based upon the given rules of alignment, cohesion, separation, and any active waypoints:
	void ApplyMovement(float DeltaTime, 
		FVector Alignment = FVector::ZeroVector, 
		FVector Cohesion = FVector::ZeroVector, 
		FVector Separation = FVector::ZeroVector, 
		FVector WaypointAttraction = FVector::ZeroVector);

	// Returns the Boid's Detection Radius
	float GetDetectionRadius();

	// Set the Boid's Detection Radius
	void SetDetectionRadius(float aRadius);

	// Returns the Boid's Minimum Speed
	float GetMinimumSpeed();

	// Set the Boid's Minimum Speed
	void SetMinimumSpeed(float aSpeed);

	// Returns the Boid's Maximum Speed
	float GetMaximumSpeed();

	// Set the Boid's Maximum Speed
	void SetMaximumSpeed(float aSpeed);


	// DEBUGGING //

	// Draws a debug arrow in the direction Cohesion is steering the Boid
	UFUNCTION(BlueprintCallable)
	void DebugCohesion(FVector Cohesion);

	// Draws a debug arrow in the direction Alignment is steering the Boid
	UFUNCTION(BlueprintCallable)
	void DebugAlignment(FVector Alignment);

	// Draws a debug arrow in the direction Separation is steering the Boid
	UFUNCTION(BlueprintCallable)
	void DebugSeparation(FVector Separation);

	// Draws a debug arrow in the direction the Waypoint is steering the Boid
	UFUNCTION(BlueprintCallable)
	void DebugWaypointAttraction(FVector WaypointAttraction);

	// Draws a debug sphere around the Boid based upon its detection radius
	UFUNCTION(BlueprintCallable)
	void DebugDrawDetectionRadius();
};
