// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Waypoint.generated.h"

UCLASS()
class BOIDS_ASSIGNMENT1_GB_API AWaypoint : public AActor
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Attraction Values", meta = (AllowPrivateAccess = "true"))
	float myAttractionRadius{ 2000.0f };

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

	UFUNCTION(BlueprintCallable)
	float GetAttractionRadius();

	UFUNCTION(BlueprintCallable)
	void SetAttractionRadius(float aRadius);

	UFUNCTION(BlueprintCallable)
	void DebugDrawAttractionRadius();
};
