// Fill out your copyright notice in the Description page of Project Settings.


#include "Gunship.h"

AGunship::AGunship()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// scale the box mesh to be an appropriate hitbox for a gunship
	VisualMesh->SetWorldScale3D(FVector(30.0f, 10.0f, 10.0f));
	// set simulate physics to false
	VisualMesh->SetSimulatePhysics(false);

	// set up the camera in a position appropriate for the viewfinder of an AC-130
	Viewfinder = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Viewfinder->SetupAttachment(VisualMesh); // don't think we need to do this when we're using AttachToComponent? probably redundant	Viewfinder->SetRelativeLocation(FVector(0.0f, -55.0f, 0.0f)); // have to use this when the camera inherits the VisualMesh's scaling as we subclass from Pawn instead
	Viewfinder->SetRelativeLocation(FVector(0.0f, -55.0f, 0.0f)); // have to use this when the camera inherits the VisualMesh's scaling as we subclass from Pawn instead
	FRotator ViewfinderRotation = GetActorRotation();
	ViewfinderRotation.Add(0.0, -90.0, 0.0);
	Viewfinder->SetRelativeRotation(ViewfinderRotation);

	// set up the spline component
	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	//SplineComponent->SetupAttachment(VisualMesh);

	// scale the camera and spline back to their original sizes by dividing by visualmesh's scale
	Viewfinder->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f) / FVector(30.0f, 10.0f, 10.0f));
	//SplineComponent->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f) / FVector(30.0f, 10.0f, 10.0f));

	// gunship is not hostile, default the flag to false
	Hostile = false;

	// default main weapon to small weapon
	EquippedWeapon = &SmallWeapon;
	// tie index to associated weapon set above (0 = small, 1 = medium, 2 = large)
	WeaponIndex = 0;
}

void AGunship::BeginPlay()
{
	Super::BeginPlay();

	check(GEngine != nullptr);

	// Display a debug message for five seconds
	// The -1 "Key" value argument prevents the message from being updated or refreshed
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::White, TEXT("We are using Gunship uwu"));

	// spawn the HUD if we have one set up
	if (IsValid(HUDClass)) {
		HUDWidget = CreateWidget(GetWorld(), HUDClass);
		if (HUDWidget) {
			HUDWidget->AddToViewport();
		}
	}

	// set input mode for the controller which this should be set to posses in the editor
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetInputMode(FInputModeGameOnly());
}

void AGunship::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// move our gunship along the currently set spline
	if (SplineComponent && SplineComponent->GetSplineLength() != 0.0f) {
		// log spline length
		//if (GEngine) {
		//	// Display a debug message for five seconds
		//	// The -1 "Key" value argument prevents the message from being updated or refreshed
		//	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::White, FString::Printf(TEXT("Spline Length: %f"), SplineComponent->GetSplineLength()));
		//}
		SplinePos += SplineSpeed;
		if (SplinePos >= SplineComponent->GetSplineLength()) {
			SplinePos = 0.0f;
		}
		// log spline position
		//if (GEngine) {
		//	// Display a debug message for five seconds
		//	// The -1 "Key" value argument prevents the message from being updated or refreshed
		//	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::White, FString::Printf(TEXT("SplinePos: %f"), SplinePos));
		//}
		FVector SplinePoint = SplineComponent->GetLocationAtDistanceAlongSpline(SplinePos, ESplineCoordinateSpace::World);
		// log point along spline
		//if (GEngine) {
		//	// Display a debug message for five seconds
		//	// The -1 "Key" value argument prevents the message from being updated or refreshed
		//	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::White, FString::Printf(TEXT("X: %f, Y: %f, Z: %f"), SplinePoint.X, SplinePoint.Y, SplinePoint.Z));
		//}
		// location
		VisualMesh->SetWorldLocation(SplinePoint);
		// rotation
		VisualMesh->SetWorldRotation(SplineComponent->GetRotationAtDistanceAlongSpline(SplinePos, ESplineCoordinateSpace::World));
	}

	// Zoom in if ZoomIn button is down, zoom back out if it's not
	if (ZoomedIn)
	{
		ZoomFactor += DeltaTime / 0.5f;         //Zoom in over half a second
	}
	else
	{
		ZoomFactor -= DeltaTime / 0.25f;        //Zoom out over a quarter of a second
	}
	ZoomFactor = FMath::Clamp<float>(ZoomFactor, 0.0f, 1.0f);
	Viewfinder->FieldOfView = FMath::Lerp<float>(90.0f, 60.0f, ZoomFactor);

	// Rotate our viewfinder within the limits of its ability
	FRotator NewViewfinderRotation = Viewfinder->GetRelativeRotation();
	NewViewfinderRotation.Yaw = FMath::Clamp(NewViewfinderRotation.Yaw +  ViewfinderInput.X, -135.0f, -45.0f);
	NewViewfinderRotation.Pitch = FMath::Clamp(NewViewfinderRotation.Pitch + ViewfinderInput.Y, -30.0f, 0.0f);
	Viewfinder->SetRelativeRotation(NewViewfinderRotation);

	// update current weapon's muzzle location and rotation based on viewfinder
	EquippedWeapon->MuzzleLocation = Viewfinder->GetComponentLocation();
	EquippedWeapon->MuzzleDirection = Viewfinder->GetComponentRotation();
}

void AGunship::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// set up this class as the player input component and attach the project's custom actions/axis to our handlers
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction<FWeaponFiringDelegate>("Fire", IE_Pressed, this, &ATarget::SetWeaponFiring, true);
	PlayerInputComponent->BindAction<FWeaponFiringDelegate>("Fire", IE_Released, this, &ATarget::SetWeaponFiring, false);
	PlayerInputComponent->BindAction<FWeaponCycleDelegate>("WeaponCycleUp", IE_Pressed, this, &AGunship::WeaponCycle, true);
	PlayerInputComponent->BindAction<FWeaponCycleDelegate>("WeaponCycleDown", IE_Pressed, this, &AGunship::WeaponCycle, false);
	PlayerInputComponent->BindAction("Zoom", IE_Pressed, this, &AGunship::ZoomIn);
	PlayerInputComponent->BindAction("Zoom", IE_Released, this, &AGunship::ZoomOut);
	PlayerInputComponent->BindAxis("ViewfinderPitch", this, &AGunship::PitchViewfinder);
	PlayerInputComponent->BindAxis("ViewfinderYaw", this, &AGunship::YawViewfinder);

}

void AGunship::PitchViewfinder(float AxisValue)
{
	ViewfinderInput.Y = AxisValue;
}

void AGunship::YawViewfinder(float AxisValue)
{
	ViewfinderInput.X = AxisValue;
}

void AGunship::ZoomIn()
{
	ZoomedIn = true;
}

void AGunship::ZoomOut()
{
	ZoomedIn = false;
}

void AGunship::WeaponCycle(bool CycleUp)
{
	if (CycleUp) {
		if (WeaponIndex == 2) {
			WeaponIndex = 0;
		}
		else {
			WeaponIndex++;
		}
	}
	else {
		if (WeaponIndex == 0) {
			WeaponIndex = 2;
		}
		else {
			WeaponIndex--;
		}
	}
	switch (WeaponIndex) {
	case 0:
		SetEquippedWeapon(SmallWeapon);
		break;
	case 1:
		SetEquippedWeapon(MediumWeapon);
		break;
	case 2:
		SetEquippedWeapon(LargeWeapon);
		break;
	default:
		break;
	}
}
