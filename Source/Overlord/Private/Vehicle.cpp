// Fill out your copyright notice in the Description page of Project Settings.


#include "Vehicle.h"

AVehicle::AVehicle()
{
	// set the default mesh of a Vehicle
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
	if (SphereVisualAsset.Succeeded())
	{
		VisualMesh->SetStaticMesh(SphereVisualAsset.Object);
		VisualMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	}

	// set up the movement component
	VehicleMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement"));
	VehicleMovement->UpdatedComponent = RootComponent;
	VehicleMovement->MaxSpeed = SpeedLimit;
	VehicleMovement->Acceleration = AccelLimit;
	VehicleMovement->Deceleration = AccelLimit;
}

void AVehicle::BeginPlay()
{
	// this is needed for destroy calls to work
	Super::BeginPlay();
}

void AVehicle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
