#include "Boid.h"

// Sets default values
ABoid::ABoid()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	// Root Component Setup:
	myRoot = CreateDefaultSubobject<USceneComponent>(myRootComponentName);
	RootComponent = myRoot;

	// Static Mesh Component Setup (Boid Body):
	myBoidBody = CreateDefaultSubobject<UStaticMeshComponent>(myBoidBodyName);
	myBoidBody->AttachToComponent(myRoot, FAttachmentTransformRules::KeepRelativeTransform);
	myBoidBody->SetRelativeRotation(FRotator(0.0, 90.0, -90.0));
}

// Called when the game starts or when spawned
void ABoid::BeginPlay()
{
	Super::BeginPlay();
	
	// Set the Boid's initial velocity along its forward direction:
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

// Called to apply movement based upon the given rules of alignment, cohesion, separation, and any active waypoints:
void ABoid::ApplyMovement(float DeltaTime, FVector Alignment, FVector Cohesion, FVector Separation, FVector WaypointAttraction)
{
	// Update Position:
	AddActorWorldOffset((myVelocity * DeltaTime));

	// Update Rotation:
	SetActorRotation(FMath::RInterpTo(GetActorRotation(), FRotator(myVelocity.ToOrientationQuat()), DeltaTime, 0.5)); // Smoothed rotation.

	// Apply Updates to Acceleration:
	myAcceleration += Alignment;
	myAcceleration += Cohesion;
	myAcceleration += Separation;
	myAcceleration += WaypointAttraction;

	// Update Velocity:
	myVelocity += (myAcceleration * DeltaTime);

	// Clamp Velocity Between Min & Max Speed:
	myVelocity = myVelocity.GetClampedToSize(myMinSpeed, myMaxSpeed);

	// Reset Acceleration back to Zero:
	myAcceleration = FVector::ZeroVector;
}

// Returns the Boid's Detection Radius
float ABoid::GetDetectionRadius()
{
	return myDetectionRadius;
}

// Set the Boid's Detection Radius
void ABoid::SetDetectionRadius(float aRadius)
{
	myDetectionRadius = aRadius > 0 ? aRadius : myDetectionRadius; // Ensures the given radius is greater than or equal to 0; otherwise sets it back to the original value.
}

// Returns the Boid's Minimum Speed
float ABoid::GetMinimumSpeed()
{
	return myMinSpeed;
}

// Set the Boid's Minimum Speed
void ABoid::SetMinimumSpeed(float aSpeed)
{
	myMinSpeed = aSpeed >= 1.0 ? aSpeed : myMinSpeed; // Ensures the given speed is greater than or equal to 1.0; otherwise sets it back to the original value.
}

// Returns the Boid's Maximum Speed
float ABoid::GetMaximumSpeed()
{
	return myMaxSpeed;
}

// Set the Boid's Maximum Speed
void ABoid::SetMaximumSpeed(float aSpeed)
{
	myMaxSpeed = aSpeed >= 1.0 ? aSpeed : myMaxSpeed; // Ensures the given speed is greater than or equal to 1.0; otherwise sets it back to the original value.
}

void ABoid::DebugCohesion(FVector Cohesion = FVector(0.f))
{
	if (const UWorld* world = GetWorld())
	{
		DrawDebugDirectionalArrow(world, GetActorLocation(), GetActorLocation() + Cohesion * myDebugLineMultiplier, 15.0f, FColor::Emerald, false, -1, 0, 3);
		//DrawDebugDirectionalArrow(world, GetActorLocation(), GetActorLocation() + Cohesion, 15.0f, FColor::Emerald, false, -1, 0, 3);

		//GEngine->AddOnScreenDebugMessage(FMath::Rand(), world->GetDeltaSeconds(), FColor::Emerald, FString::Printf(TEXT("Cohesion Vector: %s"), *Cohesion.ToString()));
	}
}

void ABoid::DebugAlignment(FVector Alignment)
{
	if (const UWorld* world = GetWorld())
	{
		DrawDebugDirectionalArrow(world, GetActorLocation(), GetActorLocation() + Alignment * myDebugLineMultiplier, 15.0f, FColor::Blue, false, -1, 0, 3);
		//DrawDebugDirectionalArrow(world, GetActorLocation(), GetActorLocation() + Alignment, 15.0f, FColor::Blue, false, -1, 0, 3);

		//GEngine->AddOnScreenDebugMessage(FMath::Rand(), world->GetDeltaSeconds(), FColor::Blue, FString::Printf(TEXT("Alignment Vector: %s"), *Alignment.ToString()));
	}
}

void ABoid::DebugSeparation(FVector Separation)
{
	if (const UWorld* world = GetWorld())
	{
		DrawDebugDirectionalArrow(world, GetActorLocation(), GetActorLocation() + Separation * myDebugLineMultiplier, 15.0f, FColor::Red, false, -1, 0, 3);
		//DrawDebugDirectionalArrow(world, GetActorLocation(), GetActorLocation() + Separation, 15.0f, FColor::Red, false, -1, 0, 3);

		//GEngine->AddOnScreenDebugMessage(FMath::Rand(), world->GetDeltaSeconds(), FColor::Red, FString::Printf(TEXT("Separation Vector: %s"), *Separation.ToString()));
	}
}

void ABoid::DebugWaypointAttraction(FVector WaypointAttraction)
{
	if (const UWorld* world = GetWorld())
	{
		DrawDebugDirectionalArrow(world, GetActorLocation(), GetActorLocation() + WaypointAttraction * myDebugLineMultiplier, 15.0f, FColor::Orange, false, -1, 0, 3);
		//DrawDebugDirectionalArrow(world, GetActorLocation(), GetActorLocation() + WaypointAttraction, 15.0f, FColor::Red, false, -1, 0, 3);

		//GEngine->AddOnScreenDebugMessage(FMath::Rand(), world->GetDeltaSeconds(), FColor::Orange, FString::Printf(TEXT("Waypoint Attraction Vector: %s"), *WaypointAttraction.ToString()));
	}
}

void ABoid::DebugDrawDetectionRadius()
{
	if (const UWorld* world = GetWorld())
	{
		DrawDebugSphere(world, this->GetActorLocation(), myDetectionRadius, 20, FColor::White);
	}
}

