// Fill out your copyright notice in the Description page of Project Settings.


#include "Boid.h"

// Sets default values
ABoid::ABoid()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set the Boid's velocity to a random unit vector as its velocity:
	myVelocity = FMath::VRand();

}

// Called when the game starts or when spawned
void ABoid::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABoid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	// Add the current velocity to the Boid's position each frame: #Todo: [HAVE NOT ACCOUNTED FOR DELTA TIME YET!]
	//AddActorWorldOffset(myVelocity);

	// Add acceleration to the Boid's velocity each frame:
	//myVelocity += myAcceleration;

}

// Called to bind functionality to input
void ABoid::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

