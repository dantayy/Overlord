// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Target.h"
#include "Human.generated.h"

// subclass of Target, to be bound to humanoid meshs and potentially shoot at other Targets
UCLASS()
class OVERLORD_API AHuman : public ATarget
{
	GENERATED_BODY()
	
public:
	// constructor
	AHuman();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
