// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_MoveVehicle.h"

UBTTask_MoveVehicle::UBTTask_MoveVehicle(const FObjectInitializer& ObjectInitializer)
{
	bCreateNodeInstance = true;
	NodeName = "Move Vehicle";
}

EBTNodeResult::Type UBTTask_MoveVehicle::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// get the blackboard
	const UBlackboardComponent* OwnerBlackboard = OwnerComp.GetBlackboardComponent();
	// get the controller tied to the tree we're being run by
	AAIController* OwnerController = OwnerComp.GetAIOwner();
	// abort if either don't return valid
	if (!OwnerController || !OwnerBlackboard) {
		return EBTNodeResult::Failed;
	}

	// grab Vehicle that should have PointPath SmartObject tied to it
	AWheeledVehicle* Vehicle = Cast<AWheeledVehicle>(OwnerController->GetPawn());
	if (!Vehicle) {
		return EBTNodeResult::Failed;
	}

	// cast PointPath SmartObject from Vehicle's SmartObjectVar
	APointPath* PathRef = Cast<APointPath>(Vehicle->SmartObject);
	// abort if no SmartObject set or SmartObject is not PointPath
	if (!PathRef || PathRef->Locations.Num() < 1) {
		return EBTNodeResult::Failed;
	}

	// set main vars on first execute with a path in hand
	if (FirstExecute) {
		// amount to increment the position along the spline 
		GoalDifference = PathRef->Path->GetSplineLength() / 20;
		// set initial goal 2 goal-diffs away 
		GoalDistance = GoalDifference * 2;
		// set initial goal world position
		GoalPos = PathRef->Path->GetWorldLocationAtDistanceAlongSpline(GoalDistance);
		// disable the flag
		FirstExecute = false;
	}
	// get vector from vehicle to goal, zeroing out Z for angle/distance calculations
	FVector GoalVec = GoalPos - Vehicle->GetActorTransform().GetLocation();
	GoalVec.Z = 0;
	// calc length from goal world pos to vehicle world pos
	float LengthFromGoal = GoalVec.Length();
	// if close enough, update the goal world pos
	if (LengthFromGoal <= UpdateLength) {
		// increment goal spline distance
		GoalDistance += GoalDifference;
		// if exceeding length of spline, reset to beginning of spline
		if (GoalDistance >= PathRef->Path->GetSplineLength()) {
			GoalDistance = 0.0f;
		}
		// set goal world pos
		GoalPos = PathRef->Path->GetWorldLocationAtDistanceAlongSpline(GoalDistance);
		// recalculate goal vector
		GoalVec = GoalPos - Vehicle->GetActorTransform().GetLocation();
		GoalVec.Z = 0;
		LengthFromGoal = GoalVec.Length();
	}

	// calc angle between forward of vehicle and to-goal vector
	//FVector VehForward = Vehicle->GetActorForwardVector();
	//VehForward.Z = 0;
	//float GoalAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(GoalVec, VehForward) /
	//	LengthFromGoal * VehForward.Length()));
	FVector VehFrontLeft = Vehicle->GetWheelLocation(0);
	float GoalLeftDistance = FVector(GoalPos - VehFrontLeft).Length();
	FVector VehFrontRight = Vehicle->GetWheelLocation(1);
	float GoalRightDistance = FVector(GoalPos - VehFrontRight).Length();

	// determine turn
	if (GoalLeftDistance < GoalRightDistance) {
		Vehicle->SetSteering(FMath::Lerp(Vehicle->GetSteering(), -0.6f, 0.3f));
	}
	else if (GoalRightDistance < GoalLeftDistance) {
		Vehicle->SetSteering(FMath::Lerp(Vehicle->GetSteering(), 0.6f, 0.3f));
	}

	// determine acceleration
	if (Vehicle->GetSpeed() < Vehicle->MaxSpeed) {
		Vehicle->SetThrottle(FMath::Lerp(Vehicle->GetThrottle(), 1.0f, 0.3f));
	}
	else {
		Vehicle->SetThrottle(0.0f);
	}

	// determine next acceleration input vector based on above angle
	//FVector Acceleration;
	//if (GoalAngle > UpdateAngle) {
	//	Acceleration = GoalVec.GetClampedToSize(Vehicle->AccelLimit, Vehicle->AccelLimit);
	//}
	//else {
	//	Acceleration = VehForward.GetClampedToSize(Vehicle->AccelLimit, Vehicle->AccelLimit);
	//}

	// vehicles cannot accelerate upward or downward on their own
	//Acceleration.Z = 0;

	// rotate vehicle towards acceleration
	//FRotator VehicleRot = Vehicle->GetActorRotation();
	//FRotator GoalRot = Acceleration.ToOrientationRotator();
	//if (VehicleRot.Yaw != GoalRot.Yaw) {
	//	VehicleRot.Yaw = FMath::Lerp(VehicleRot.Yaw, GoalRot.Yaw, .4);
	//	Vehicle->VisualMesh->SetWorldRotation(VehicleRot);
	//}

	// apply acceleration to vehicle for next movement update
	//Vehicle->GetMovementComponent()->AddInputVector(Acceleration, true);
	//Vehicle->GetMovementComponent()->RequestDirectMove(Acceleration, true);
	//Vehicle->AccelVector = Acceleration;

	return EBTNodeResult::Succeeded;
}
