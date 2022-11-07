// Fill out your copyright notice in the Description page of Project Settings.


#include "Human.h"

AHuman::AHuman()
{
	// set the default mesh of a Human
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CapsuleVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_WideCapsule.Shape_WideCapsule"));
	if (CapsuleVisualAsset.Succeeded())
	{
		VisualMesh->SetStaticMesh(CapsuleVisualAsset.Object);
		VisualMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	}
}

void AHuman::BeginPlay()
{
	// this is needed for destroy calls to work
	Super::BeginPlay();
}

void AHuman::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
