// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Boid.generated.h"

constexpr auto DEBUG_MODE_ENABLED = 0;

UCLASS()
class BOIDS_ASSIGNMENT1_GB_API ABoid : public APawn
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly, Category = "Boid Defaults", meta = (AllowPrivateAccess = "true"))
	float myDetectionRadius{ 100.0 };

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly, Category = "Boid Defaults", meta = (AllowPrivateAccess = "true"))
	float myMaxSpeed{ 1.0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid Defaults", meta = (AllowPrivateAccess = "true"))
	FVector myVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid Defaults", meta = (AllowPrivateAccess = "true"))
	FVector myAcceleration;


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



};
