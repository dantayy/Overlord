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
	AVehicle* Vehicle = Cast<AVehicle>(OwnerController->GetPawn());
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
	else {
		// calc length from goal world pos to vehicle world pos
		float LengthFromGoal = FVector(GoalPos - Vehicle->GetActorTransform().GetLocation()).Length();

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
		}
	}

	// calc angle between forward of vehicle and to-goal vector
	float GoalAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(GoalPos - Vehicle->GetActorTransform().GetLocation(), Vehicle->GetActorForwardVector()) / 
		FVector(GoalPos - Vehicle->GetActorTransform().GetLocation()).Length() * Vehicle->GetActorForwardVector().Length()));

	// determine next acceleration input vector based on above angle
	FVector Acceleration;
	if (GoalAngle > UpdateAngle) {
		Acceleration = FVector(GoalPos - Vehicle->GetActorTransform().GetLocation()).GetClampedToMaxSize(Vehicle->AccelLimit);
	}
	else {
		Acceleration = Vehicle->GetActorForwardVector().GetClampedToSize(Vehicle->AccelLimit, Vehicle->AccelLimit);
	}

	// vehicles cannot accelerate upward or downward on their own
	//Acceleration.Z = 0;

	// rotate vehicle towards acceleration
	FQuat VehicleRot = Vehicle->GetActorQuat();
	FQuat GoalRot = Acceleration.ToOrientationQuat();
	if (VehicleRot != GoalRot) {
		Vehicle->VisualMesh->SetWorldRotation(FQuat::Slerp(VehicleRot, GoalRot, 0.4));
	}

	// apply acceleration to vehicle for next movement update
	Vehicle->GetMovementComponent()->AddInputVector(Acceleration, true);

	return EBTNodeResult::Succeeded;
}
