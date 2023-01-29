// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Boid.generated.h"

//#define DEBUG_MODE_ENABLED 1

// Global Constexpr Variable Used As A Debugging Flag:
constexpr auto DEBUG_MODE_ENABLED = 1;

UCLASS()
class BOIDS_ASSIGNMENT1_GB_API ABoid : public APawn
{
	GENERATED_BODY()

	// Root Component
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta = (AllowPrivateAccess = "true")) USceneComponent* myRoot;
	UPROPERTY() FName myRootComponentName = "Root";

	// Boid Visual Representation (Cone)
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta = (AllowPrivateAccess = "true")) UStaticMeshComponent* myBoidBody;
	UPROPERTY(EditDefaultsOnly) FName myBoidBodyName = " Boid Body";

	// Detection Radius for use later
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Boid | Movement", meta = (AllowPrivateAccess = "true"))
	float myDetectionRadius{ 100.0 };

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly, Category = "Boid | Movement", meta = (AllowPrivateAccess = "true"))
	float myMinSpeed{ 30.0f };

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly, Category = "Boid | Movement", meta = (AllowPrivateAccess = "true"))
	float myMaxSpeed{ 70.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid | Movement", meta = (AllowPrivateAccess = "true"))
	FVector myVelocity {0.f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid | Movement", meta = (AllowPrivateAccess = "true"))
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


	// MOVEMENT //
	void ApplyMovement(float DeltaTime, 
		FVector Alignment = FVector::ZeroVector, 
		FVector Cohesion = FVector::ZeroVector, 
		FVector Separation = FVector::ZeroVector, 
		FVector WaypointAttraction = FVector::ZeroVector);

	void ApplyWaypointMovement();

	// Returns the Boid's Detection Radius
	float GetDetectionRadius();

	// Set the Boid's Detection Radius
	void SetDetectionRadius(float aRadius);

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
