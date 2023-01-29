// Fill out your copyright notice in the Description page of Project Settings.


#include "Boid.h"

// Sets default values
ABoid::ABoid()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	// Root Component Setup:
	myRoot = CreateDefaultSubobject<USceneComponent>(myRootComponentName);
	RootComponent = myRoot;

	myBoidBody = CreateDefaultSubobject<UStaticMeshComponent>(myBoidBodyName);
	myBoidBody->AttachToComponent(myRoot, FAttachmentTransformRules::KeepRelativeTransform);
	myBoidBody->SetRelativeRotation(FRotator(0.0, 90.0, -90.0));


	// Set the Boid's velocity to a random unit vector as its velocity:
	//myVelocity = FMath::VRand();
	
}

// Called when the game starts or when spawned
void ABoid::BeginPlay()
{
	Super::BeginPlay();
	myVelocity = GetActorForwardVector();
}

// Called every frame
void ABoid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABoid::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

// #TODO: Should be refactored, currently should only be called by Flock class for movement:
void ABoid::ApplyMovement(float DeltaTime, FVector Alignment, FVector Cohesion, FVector Separation, FVector WaypointAttraction)
{
	// Update Position:
	//SetActorLocation(GetActorLocation() + (myVelocity * DeltaTime)); // Essentially the same as adding actor world offset below
	AddActorWorldOffset((myVelocity * DeltaTime));

	//NEW TECHNIQUE
	//myVelocity += Cohesion;
	//myVelocity += Alignment;
	//myVelocity += Separation;
	//myVelocity = myVelocity.GetClampedToSize(myMinSpeed, myMaxSpeed); //uNSURE IF NEEDED
	//AddActorWorldOffset((myVelocity * DeltaTime));

	// Update Rotation:
	//SetActorRotation(myVelocity.ToOrientationQuat()); // Directly Set
	SetActorRotation(FMath::RInterpTo(GetActorRotation(), FRotator(myVelocity.ToOrientationQuat()), DeltaTime, 0.5)); // Added Smoothing

	// Apply Updates to Acceleration: // #TODO: Not sure if I should add this or not
	//FVector Sum = (Alignment + Cohesion + Separation) / 2.0; //NEED TO DIVIDE BY NUMBER OF ACTIVE RULES
	//myAcceleration += Sum;

	// Alternate method attempt of just adding all steering rules to the acceleration at once:
	myAcceleration += Alignment;
	myAcceleration += Cohesion;
	myAcceleration += Separation;
	myAcceleration += WaypointAttraction;

	// Update Velocity:
	myVelocity += (myAcceleration * DeltaTime);

	// Clamp Velocity Between Min & Max Speed:
	myVelocity = myVelocity.GetClampedToSize(myMinSpeed, myMaxSpeed); //uNSURE IF NEEDED

	if (DEBUG_MODE_ENABLED)
	{
		if (UWorld* world = GetWorld())
		{
			GEngine->AddOnScreenDebugMessage(FMath::Rand(), world->GetDeltaSeconds(), FColor::White, FString::Printf(TEXT("%s's Velocity: %s"), *this->GetName(), *myVelocity.ToString()));
			//GEngine->AddOnScreenDebugMessage(FMath::Rand(), world->GetDeltaSeconds(), FColor::Purple, FString::Printf(TEXT("%s's Acceleration: %s"), *this->GetName(), *myAcceleration.ToString()));
		}
	}
	// Reset Acceleration back to Zero:
	myAcceleration = FVector::ZeroVector;

	//new technique
	//myVelocity = FVector::ZeroVector;
}

void ABoid::ApplyWaypointMovement()
{

}

float ABoid::GetDetectionRadius()
{
	return myDetectionRadius;
}

void ABoid::SetDetectionRadius(float aRadius)
{
	myDetectionRadius = aRadius > 0 ? aRadius : myDetectionRadius;
}

void ABoid::DebugCohesion(FVector Cohesion = FVector(0.f))
{
	if (const UWorld* world = GetWorld())
	{
		DrawDebugDirectionalArrow(world, GetActorLocation(), GetActorLocation() + Cohesion * myDebugLineMultiplier, 15.0f, FColor::Emerald, false, -1, 0, 3);
		//DrawDebugDirectionalArrow(world, GetActorLocation(), GetActorLocation() + Cohesion, 15.0f, FColor::Emerald, false, -1, 0, 3);

		GEngine->AddOnScreenDebugMessage(FMath::Rand(), world->GetDeltaSeconds(), FColor::Emerald, FString::Printf(TEXT("Cohesion Vector: %s"), *Cohesion.ToString()));
	}
}

void ABoid::DebugAlignment(FVector Alignment)
{
	if (const UWorld* world = GetWorld())
	{
		DrawDebugDirectionalArrow(world, GetActorLocation(), GetActorLocation() + Alignment * myDebugLineMultiplier, 15.0f, FColor::Blue, false, -1, 0, 3);
		//DrawDebugDirectionalArrow(world, GetActorLocation(), GetActorLocation() + Alignment, 15.0f, FColor::Blue, false, -1, 0, 3);

		GEngine->AddOnScreenDebugMessage(FMath::Rand(), world->GetDeltaSeconds(), FColor::Blue, FString::Printf(TEXT("Alignment Vector: %s"), *Alignment.ToString()));
	}
}

void ABoid::DebugSeparation(FVector Separation)
{
	if (const UWorld* world = GetWorld())
	{
		DrawDebugDirectionalArrow(world, GetActorLocation(), GetActorLocation() + Separation * myDebugLineMultiplier, 15.0f, FColor::Red, false, -1, 0, 3);
		//DrawDebugDirectionalArrow(world, GetActorLocation(), GetActorLocation() + Separation, 15.0f, FColor::Red, false, -1, 0, 3);

		GEngine->AddOnScreenDebugMessage(FMath::Rand(), world->GetDeltaSeconds(), FColor::Red, FString::Printf(TEXT("Separation Vector: %s"), *Separation.ToString()));
	}
}

void ABoid::DebugWaypointAttraction(FVector WaypointAttraction)
{
	if (const UWorld* world = GetWorld())
	{
		DrawDebugDirectionalArrow(world, GetActorLocation(), GetActorLocation() + WaypointAttraction * myDebugLineMultiplier, 15.0f, FColor::Orange, false, -1, 0, 3);
		//DrawDebugDirectionalArrow(world, GetActorLocation(), GetActorLocation() + WaypointAttraction, 15.0f, FColor::Red, false, -1, 0, 3);

		GEngine->AddOnScreenDebugMessage(FMath::Rand(), world->GetDeltaSeconds(), FColor::Orange, FString::Printf(TEXT("Waypoint Attraction Vector: %s"), *WaypointAttraction.ToString()));
	}
}

void ABoid::DebugDrawDetectionRadius()
{
	if (const UWorld* world = GetWorld())
	{
		DrawDebugSphere(world, this->GetActorLocation(), myDetectionRadius, 20, FColor::White);
	}
}

