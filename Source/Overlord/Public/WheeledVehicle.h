// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Vehicle.h"
#include "ChaosWheeledVehicleMovementComponent.h"
#include "WheeledVehicle.generated.h"

/**
 * 
 */
UCLASS()
class OVERLORD_API AWheeledVehicle : public AVehicle
{
	GENERATED_BODY()

public:

	// constructor
	AWheeledVehicle();

	// skeleton mesh component to replace static mesh component
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	USkeletalMeshComponent* SkeletonComponent = nullptr;

	// skeleton mesh to fill with a wheeled vehicle
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	USkeletalMesh* Skeleton = nullptr;

	// Wheeled vehicle movement component to control our vehicle with
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	UChaosWheeledVehicleMovementComponent* WheeledVehicleMovement = nullptr;

	// max speed of a wheeled vehicle
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MaxSpeed = 40.0f;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// vehicle control functions
	void SetThrottle(float Val);
	void SetSteering(float Val);
	void SetBreaking(bool Breaking);
	float GetSpeed();
	float GetThrottle();
	float GetSteering();
	bool GetBreaking();
	FVector GetWheelLocation(UINT WheelNum);
};
