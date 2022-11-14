// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Target.h"
#include "Components/BoxComponent.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Camera/CameraComponent.h"
//#include "GameFramework/PlayerController.h"
#include "Projectile.h"
#include "Gunship.generated.h"

// Player character pawn, aka the viewfinder of an AC130 Gunship
UCLASS()
class OVERLORD_API AGunship : public ATarget
{
	GENERATED_BODY()
	
public:
	// Constructor
	AGunship();

	// location from which the Projectile will fire from
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FVector MuzzleOffset;

	// spline path for gunship to follow
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline")
	USplineComponent* SplineComponent;

	// camera for player to control
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Camera)
	class UCameraComponent* Viewfinder;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Projectile to be fired from the Gunship
	UPROPERTY(EditAnywhere, Category = Projectile)
	TSubclassOf<class AProjectile> ProjectileClass;

	// input for Viewfinder
	FVector2D ViewfinderInput;

	// zoom factor for viewfinder
	float ZoomFactor = 0.0f;
	// zoom flag
	bool ZoomedIn = false;

	// spline lerp position (to be cycled between 0 to 1)
	float SplinePos = 0.0f;

	// ammount to increment spline position by every tick (aka spline speed)
	float SplineSpeed = 5.0f;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Fires a Projectile when triggered
	UFUNCTION()
	void Fire();

	// Rotates the Viewfinder around X when triggered
	UFUNCTION()
	void PitchViewfinder(float AxisValue);
	// Rotates the Viewfinder around Z when triggered
	UFUNCTION()
	void YawViewfinder(float AxisValue);
	// Zooms in the Viewfinder when triggered
	UFUNCTION()
	void ZoomIn();
	// Zooms out the Viewfinder when triggered
	UFUNCTION()
	void ZoomOut();
};
