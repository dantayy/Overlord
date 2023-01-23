// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SmartObject.h"
#include "PointPath.generated.h"

/**
 * Smart Object that AI can move around a set path with
 */
UCLASS()
class OVERLORD_API APointPath : public ASmartObject
{
	GENERATED_BODY()

public:

	// constructor
	APointPath();

	// run when loaded into world
	virtual void BeginPlay() override;

	// spline component to guide AI with
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SplinePath")
	class USplineComponent* Path;

	// sets Locations with world positions
	void GetSplinePoints();

	// locations for AI to move to
	TArray<FVector> Locations;
	
};
