// Fill out your copyright notice in the Description page of Project Settings.


#include "WheeledVehicle.h"

AWheeledVehicle::AWheeledVehicle()
{
	SkeletonComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeleton"));

	SkeletonComponent->SetSkeletalMesh(Skeleton);

	RootComponent = SkeletonComponent;

	//WheeledVehicleMovement = CreateDefaultSubobject<UChaosVehicleMovementComponent>(TEXT("WheeledMovement"));

	//WheeledVehicleMovement->UpdatedComponent = RootComponent;
}

void AWheeledVehicle::BeginPlay()
{
	Super::BeginPlay();
}

void AWheeledVehicle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
