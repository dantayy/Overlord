// Fill out your copyright notice in the Description page of Project Settings.


#include "PointPath.h"
#include "Components/SplineComponent.h"

APointPath::APointPath()
{
	PrimaryActorTick.bCanEverTick = false;

	Path = CreateDefaultSubobject<USplineComponent>(TEXT("Path"));
	Path->SetupAttachment(RootComponent);
	Path->bDrawDebug = true;
}

void APointPath::BeginPlay()
{
	Super::BeginPlay();
	// collect locations
	GetSplinePoints();
}

void APointPath::GetSplinePoints()
{
	// add a location for each spline point
	for (int i = 0; i < Path->GetNumberOfSplinePoints(); i++)
	{
		Locations.Add(Path->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World));
	}
}
