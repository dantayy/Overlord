// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Target.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Vehicle.generated.h"

// subclass of Target, to be subclassesed and attached to vehicle meshes to drive around in levels
UCLASS()
class OVERLORD_API AVehicle : public ATarget
{
	GENERATED_BODY()

public:
	// constructor
	AVehicle();

	// vehicle movement component
	UFloatingPawnMovement* VehicleMovement;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
