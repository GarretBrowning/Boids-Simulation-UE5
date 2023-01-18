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

	// Root Component:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta = (AllowPrivateAccess = "true")) USceneComponent* myRoot;
	UPROPERTY() FName myRootComponentName = "Root";

	// Boid Visual Representation (Cone):
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta = (AllowPrivateAccess = "true")) UStaticMeshComponent* myBoidBody;
	UPROPERTY(EditDefaultsOnly) FName myBoidBodyName = " Boid Body";


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
};
