// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Target.h"
#include "Building.generated.h"

// subclass of Target, to be bound to static structure meshes that can be fractured when dropped to 0 health
UCLASS()
class OVERLORD_API ABuilding : public ATarget
{
	GENERATED_BODY()
	
public:
	// constructor
	ABuilding();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
