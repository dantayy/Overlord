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
}

void AGunship::BeginPlay()
{
	Super::BeginPlay();

	check(GEngine != nullptr);

	// Display a debug message for five seconds
	// The -1 "Key" value argument prevents the message from being updated or refreshed
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::White, TEXT("We are using Gunship uwu"));

	//SetInputMode(FInputModeGameAndUI());
}

void AGunship::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// move our gunship along the currently set spline
	if (SplineComponent->GetSplineLength() != 0.0f) {
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
}

void AGunship::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// set up this class as the player input component and attach the project's custom actions/axis to our handlers
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AGunship::Fire);
	PlayerInputComponent->BindAction("Zoom", IE_Pressed, this, &AGunship::ZoomIn);
	PlayerInputComponent->BindAction("Zoom", IE_Released, this, &AGunship::ZoomOut);
	PlayerInputComponent->BindAxis("ViewfinderPitch", this, &AGunship::PitchViewfinder);
	PlayerInputComponent->BindAxis("ViewfinderYaw", this, &AGunship::YawViewfinder);

}

void AGunship::Fire()
{
	// Attempt to fire a projectile
	if (ProjectileClass)
	{
		// log successful firing
		if (GEngine) {
			// Display a debug message for five seconds
			// The -1 "Key" value argument prevents the message from being updated or refreshed
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Firing"));
		}
		// Get the camera transform
		FVector CameraLocation = Viewfinder->GetComponentLocation();
		FRotator CameraRotation = Viewfinder->GetComponentRotation();
		//GetActorEyesViewPoint(CameraLocation, CameraRotation);

		// Set MuzzleOffset from camera space to world space
		FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);

		// Skew the aim to be slightly upwards
		FRotator MuzzleRotation = CameraRotation;
		MuzzleRotation.Pitch += 10.0f;

		// check to make sure Gunship is in an active world before spawning Projectile
		UWorld* World = GetWorld();
		if (World)
		{
			// create spawn parameters
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();

			// Spawn the projectile at the muzzle
			AProjectile* Projectile = World->SpawnActor<AProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
			if (Projectile)
			{
				// Set the projectile's initial trajectory
				FVector LaunchDirection = MuzzleRotation.Vector();
				Projectile->FireInDirection(LaunchDirection);
			}
		}
	}
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
