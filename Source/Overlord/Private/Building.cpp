// Fill out your copyright notice in the Description page of Project Settings.


#include "Building.h"

ABuilding::ABuilding()
{
	// set the default mesh of a Building
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ConeVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Cone.Shape_Cone"));
	if (ConeVisualAsset.Succeeded())
	{
		VisualMesh->SetStaticMesh(ConeVisualAsset.Object);
		VisualMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	}
}

void ABuilding::BeginPlay()
{
	// this is needed for destroy calls to work
	Super::BeginPlay();
}

void ABuilding::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
