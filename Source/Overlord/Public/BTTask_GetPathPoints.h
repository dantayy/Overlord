// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_GetPathPoints.generated.h"

/**
 * Custom behavior tree task that sets a vector equal to one of a series of spline points every time it is called
 */
UCLASS()
class OVERLORD_API UBTTask_GetPathPoints : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:

	// constructor
	UBTTask_GetPathPoints(const FObjectInitializer& ObjectInitializer);

	// main task execution code
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	// spline point index to send to blackboard vector var
	int32 index = 0;
	
};
