// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Target.h"
#include "Components/BoxComponent.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
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

	// spline path for gunship to follow
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline")
	USplineComponent* SplineComponent = nullptr;

	// camera for player to control
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Camera)
	class UCameraComponent* Viewfinder = nullptr;

	// HUD Widget
	UPROPERTY(EditAnywhere, Category = UI)
	TSubclassOf<UUserWidget> HUDClass;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = UI)
	class UUserWidget* HUDWidget;

	// weapon structs representing the small machine gun, medium and large canons a gunship can fire
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Gameplay)
	FWeapon SmallWeapon;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Gameplay)
	FWeapon MediumWeapon;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Gameplay)
	FWeapon LargeWeapon;

	// weapon index tied to currently equipped weapon
	uint8 WeaponIndex;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

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

	virtual void DestroyTarget() override;

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
	// swap's currently equipped weapon
	UFUNCTION()
	void WeaponCycle(bool CycleUp);
	// delegate for weapon cycling
	DECLARE_DELEGATE_OneParam(FWeaponCycleDelegate, bool);
	// crash the gunship/end the game
	UFUNCTION()
	void CrashGunship(AActor* Gunship);
};
