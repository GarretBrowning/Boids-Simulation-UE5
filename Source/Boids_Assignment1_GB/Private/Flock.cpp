// Fill out your copyright notice in the Description page of Project Settings.


#include "Flock.h"

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
	
}

// Called every frame
void AFlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFlock::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

