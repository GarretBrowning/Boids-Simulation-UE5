#include "Waypoint.h"

// Sets default values
AWaypoint::AWaypoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWaypoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWaypoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (bShowAttractionRadius)
	{
		DebugDrawAttractionRadius();
	}
}

// Gets the value of the Waypoint's attraction radius:
float AWaypoint::GetAttractionRadius()
{
	return myAttractionRadius;
}

// Sets the radius of the Waypoint's attraction radius based on the given value:
void AWaypoint::SetAttractionRadius(float aRadius)
{
	myAttractionRadius = aRadius > 0 ? aRadius : myAttractionRadius;
}

// Draws the attraction radius:
void AWaypoint::DebugDrawAttractionRadius()
{
	if (const UWorld* world = GetWorld())
	{
		DrawDebugSphere(world, this->GetActorLocation(), myAttractionRadius, 20, FColor::Orange);
	}
}

