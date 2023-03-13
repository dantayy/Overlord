// Fill out your copyright notice in the Description page of Project Settings.


#include "WheeledVehicle.h"

AWheeledVehicle::AWheeledVehicle()
{
	SkeletonComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeleton"));

	SkeletonComponent->SetSkeletalMesh(Skeleton);

	RootComponent = SkeletonComponent;

	//VisualMesh->DestroyComponent();

	WheeledVehicleMovement = CreateDefaultSubobject<UChaosWheeledVehicleMovementComponent>(TEXT("WheeledVehicleMovement"));

	WheeledVehicleMovement->UpdatedComponent = SkeletonComponent;

	// Torque setup
	WheeledVehicleMovement->EngineSetup.MaxRPM = 5700.0f;
	WheeledVehicleMovement->EngineSetup.TorqueCurve.GetRichCurve()->Reset();
	WheeledVehicleMovement->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(0.0f, 400.0f);
	WheeledVehicleMovement->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(1890.0f, 500.0f);
	WheeledVehicleMovement->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(5370.0f, 400.0f);

	// Steering setup
	WheeledVehicleMovement->SteeringSetup.SteeringCurve.GetRichCurve()->Reset();
	WheeledVehicleMovement->SteeringSetup.SteeringCurve.GetRichCurve()->AddKey(0.0f, 1.0f);
	WheeledVehicleMovement->SteeringSetup.SteeringCurve.GetRichCurve()->AddKey(40.0f, 0.7f);
	WheeledVehicleMovement->SteeringSetup.SteeringCurve.GetRichCurve()->AddKey(120.0f, 0.6f);

	// Differential setup
	WheeledVehicleMovement->DifferentialSetup.DifferentialType = EVehicleDifferential::AllWheelDrive;
	WheeledVehicleMovement->DifferentialSetup.FrontRearSplit = 0.65;

	// Automatic Gearbox setup
	WheeledVehicleMovement->TransmissionSetup.bUseAutomaticGears = true;
	WheeledVehicleMovement->TransmissionSetup.GearChangeTime = 0.15f;
	WheeledVehicleMovement->TransmissionSetup.TransmissionEfficiency = 1.0f;
}

void AWheeledVehicle::BeginPlay()
{
	Super::BeginPlay();
}

void AWheeledVehicle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWheeledVehicle::SetThrottle(float Val)
{
	WheeledVehicleMovement->SetThrottleInput(Val);
}

void AWheeledVehicle::SetSteering(float Val)
{
	WheeledVehicleMovement->SetSteeringInput(Val);
}

void AWheeledVehicle::SetBreaking(bool Breaking)
{
	WheeledVehicleMovement->SetHandbrakeInput(Breaking);
}

float AWheeledVehicle::GetSpeed()
{
	return WheeledVehicleMovement->GetForwardSpeed();
}

float AWheeledVehicle::GetThrottle()
{
	return WheeledVehicleMovement->GetThrottleInput();
}

float AWheeledVehicle::GetSteering()
{
	return WheeledVehicleMovement->GetSteeringInput();
}

bool AWheeledVehicle::GetBreaking()
{
	return WheeledVehicleMovement->GetHandbrakeInput();
}

FVector AWheeledVehicle::GetWheelLocation(UINT WheelNum)
{
	if (WheeledVehicleMovement->Wheels.Num() > (int)WheelNum)
	{
		return WheeledVehicleMovement->Wheels[(int)WheelNum]->Location;
	}
	return FVector();
}
