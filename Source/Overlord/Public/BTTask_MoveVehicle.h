// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "AIController.h"
#include "Components/SplineComponent.h"
#include "WheeledVehicle.h"
#include "PointPath.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "BTTask_MoveVehicle.generated.h"

/**
 * 
 */
UCLASS()
class OVERLORD_API UBTTask_MoveVehicle : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:

	// constructor
	UBTTask_MoveVehicle(const FObjectInitializer& ObjectInitializer);

	// main task execution code
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	// flag to set up initial goal on first task execution
	bool FirstExecute = true;

	// position along spline set as our current goal
	FVector GoalPos = FVector();

	// ammount to increment goal distance by when updated
	float GoalDifference = 0.0f;

	// distance along spline at which goal position is located
	float GoalDistance = 0.0f;

	// length from goal at which to update the position along the spline
	float UpdateLength = 50.0f;

	// angle at which to start re-orienting vehicle towards goal
	float UpdateAngle = 10.0f;
};
