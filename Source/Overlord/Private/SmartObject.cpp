// Fill out your copyright notice in the Description page of Project Settings.


#include "SmartObject.h"
#include "Components/ArrowComponent.h"
#include "Components/BillboardComponent.h"

// Sets default values
ASmartObject::ASmartObject()
{
	// smart objects don't need to tick, so don't let them
	PrimaryActorTick.bCanEverTick = false;

	// create the visual aid components
	Billboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard"));
	Billboard->SetupAttachment(GetRootComponent());

	FacingDirection = CreateDefaultSubobject<UArrowComponent>(TEXT("Facing Direction"));
	FacingDirection->SetupAttachment(Billboard);
}

// Called when the game starts or when spawned
void ASmartObject::BeginPlay()
{
	// run parent's functionality
	Super::BeginPlay();
	
}

// Called every frame
void ASmartObject::Tick(float DeltaTime)
{
	// run parent's functionality
	Super::Tick(DeltaTime);
}

