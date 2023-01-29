// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Flock.generated.h"

//constexpr auto DEBUG_MODE_ENABLED = 1;

class ABoid;
class AWaypoint;

UCLASS()
class BOIDS_ASSIGNMENT1_GB_API AFlock : public APawn
{
	GENERATED_BODY()

	// BlueprintReadWrite for now...
	// Collection of Boids:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	TArray<ABoid*> myBoids;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Boids | Waypoints", meta = (AllowPrivateAccess = "true"))
	TArray<AWaypoint*> myWaypoints;

	// BOID CLASS IN EDITOR USED TO SPAWN:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Boids | Spawning", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> BoidClass;


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Boids | Multiplier Value", meta = (AllowPrivateAccess = "true"))
	float CohesionMultiplier{ 1.0 };

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Boids | Multiplier Value", meta = (AllowPrivateAccess = "true"))
	float AlignmentMultiplier{ 1.0 };

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Boids | Multiplier Value", meta = (AllowPrivateAccess = "true"))
	float SeparationMultiplier{ 1.0 };

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Boids | Multiplier Value", meta = (AllowPrivateAccess = "true"))
	float WaypointAttractionMultiplier{ 1.0 };

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Boids | Detection Radius", meta = (AllowPrivateAccess = "true"))
	float IndividualDetectionRadius{ 0.f };


	// Debugging for Blueprints:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Boids | Debugging", meta = (AllowPrivateAccess = "true"))
	bool bShowAlignmentDebugLines{ false };


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Boids | Debugging", meta = (AllowPrivateAccess = "true"))
	bool bShowCohesionDebugLines{ false };

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Boids | Debugging", meta = (AllowPrivateAccess = "true"))
	bool bShowSeparationDebugLines{ false };

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Boids | Debugging", meta = (AllowPrivateAccess = "true"))
	bool bShowWaypointDebugLines{ false };

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Boids | Debugging", meta = (AllowPrivateAccess = "true"))
	bool bShowDetectionRadius{ false };

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Boids | Debugging", meta = (AllowPrivateAccess = "true"))
	bool bEnableMovement{ false };

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


	// RULES //

	// Calculating Alignment Vector:
	FVector CalculateAlignment(ABoid* aBoid);

	// Calculating Cohesion Vector:
	FVector CalculateCohesion(ABoid* aBoid);

	// Calculating Separation Vector:
	FVector CalculateSeparation(ABoid* aBoid);


	// WAYPOINT //
	FVector CalculateWaypointAttraction(ABoid* aBoid);
};
