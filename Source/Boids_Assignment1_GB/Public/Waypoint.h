// The Waypoint class is an actor within the level that provides an attraction force for all Boids to move towards.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Waypoint.generated.h"

UCLASS()
class BOIDS_ASSIGNMENT1_GB_API AWaypoint : public AActor
{
	GENERATED_BODY()

	// The radius of influence for the Waypoint.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Attraction Values", meta = (AllowPrivateAccess = "true", ClampMin = "500.0", ClampMax = "4000.0"))
	float myAttractionRadius{ 2000.0f };

	// Shows the debug sphere of the Waypoint's attraction radius (mainly for debugging purposes).
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Attraction Values", meta = (AllowPrivateAccess = "true"))
	bool bShowAttractionRadius {false};

public:	
	// Sets default values for this actor's properties
	AWaypoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Gets the value of the Waypoint's attraction radius:
	UFUNCTION(BlueprintCallable)
	float GetAttractionRadius();

	// Sets the radius of the Waypoint's attraction radius based on the given value:
	UFUNCTION(BlueprintCallable)
	void SetAttractionRadius(float aRadius);

	// Draws the attraction radius:
	UFUNCTION(BlueprintCallable)
	void DebugDrawAttractionRadius();
};
