// Fill out your copyright notice in the Description page of Project Settings.


#include "Human.h"

AHuman::AHuman()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	VisualMesh->SetSimulatePhysics(true);

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
