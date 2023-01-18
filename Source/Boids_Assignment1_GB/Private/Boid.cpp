// Fill out your copyright notice in the Description page of Project Settings.


#include "Boid.h"

// Sets default values
ABoid::ABoid()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set the Boid's velocity to a random unit vector as its velocity:
	myVelocity = FMath::VRand();
	
	// Root Component Setup:
	myRoot = CreateDefaultSubobject<USceneComponent>(myRootComponentName);
	RootComponent = myRoot;

	myBoidBody = CreateDefaultSubobject<UStaticMeshComponent>(myBoidBodyName);
	myBoidBody->AttachToComponent(myRoot, FAttachmentTransformRules::KeepRelativeTransform);
	myBoidBody->SetRelativeRotation(FRotator(0.0, 90.0, -90.0));
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

	

	//DrawDebugDirectionalArrow(World)
	
	/*if (DEBUG_MODE_ENABLED)
	{*/
	//#if DEBUG_MODE_ENABLED
	if (DEBUG_MODE_ENABLED)
	{
		GEngine->AddOnScreenDebugMessage(FMath::Rand(), GetWorld()->GetDeltaSeconds(), FColor::Yellow, "Debug mode enabled!");
	}
	//#endif
	//}

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

void ABoid::DebugCohesion(FVector Cohesion = FVector(0.f))
{
	if (const UWorld* world = GetWorld())
	{
		DrawDebugDirectionalArrow(world, GetActorLocation(), GetActorLocation() + Cohesion, 15.0f, FColor::Emerald, false, -1, 0, 3);

		GEngine->AddOnScreenDebugMessage(FMath::Rand(), world->GetDeltaSeconds(), FColor::Emerald, FString::Printf(TEXT("Cohesion Vector: %s"), *Cohesion.ToString()));
	}
}

void ABoid::DebugAlignment(FVector Alignment)
{
	if (const UWorld* world = GetWorld())
	{
		DrawDebugDirectionalArrow(world, GetActorLocation(), GetActorLocation() + Alignment, 15.0f, FColor::Blue, false, -1, 0, 3);

		GEngine->AddOnScreenDebugMessage(FMath::Rand(), world->GetDeltaSeconds(), FColor::Blue, FString::Printf(TEXT("Alignment Vector: %s"), *Alignment.ToString()));
	}
}

void ABoid::DebugSeparation(FVector Separation)
{
	if (const UWorld* world = GetWorld())
	{
		DrawDebugDirectionalArrow(world, GetActorLocation(), GetActorLocation() + Separation, 15.0f, FColor::Red, false, -1, 0, 3);

		GEngine->AddOnScreenDebugMessage(FMath::Rand(), world->GetDeltaSeconds(), FColor::Red, FString::Printf(TEXT("Alignment Vector: %s"), *Separation.ToString()));
	}
}

