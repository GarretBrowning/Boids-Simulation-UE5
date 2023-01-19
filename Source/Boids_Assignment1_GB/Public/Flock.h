// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Flock.generated.h"

//constexpr auto DEBUG_MODE_ENABLED = 1;

class ABoid;

UCLASS()
class BOIDS_ASSIGNMENT1_GB_API AFlock : public APawn
{
	GENERATED_BODY()

	// BlueprintReadWrite for now...
	// Collection of Boids:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	TArray<ABoid*> myBoids;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Boids | Multiplier Value", meta = (AllowPrivateAccess = "true"))
	float CohesionMultiplier{ 210.0 };

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Boids | Multiplier Value", meta = (AllowPrivateAccess = "true"))
	float AlignmentMultiplier{ 250.0 };

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Boids | Multiplier Value", meta = (AllowPrivateAccess = "true"))
	float SeparationMultiplier{ 215.0 };


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

	// Calculating Cohesion Vector:
	FVector CalculateSeparation(ABoid* aBoid);
};
