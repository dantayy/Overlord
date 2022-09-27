// Fill out your copyright notice in the Description page of Project Settings.


#include "Vehicle.h"

AVehicle::AVehicle()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	VisualMesh->SetSimulatePhysics(true);

	// set the default mesh of a Vehicle
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
	if (SphereVisualAsset.Succeeded())
	{
		VisualMesh->SetStaticMesh(SphereVisualAsset.Object);
		VisualMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	}
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
