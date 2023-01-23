// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_GetPathPoints.h"
#include "AIController.h"
#include "Components/SplineComponent.h"
#include "Target.h"
#include "PointPath.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"

UBTTask_GetPathPoints::UBTTask_GetPathPoints(const FObjectInitializer& ObjectInitializer)
{
	bCreateNodeInstance = true;
	NodeName = "Get Path Points";
}

EBTNodeResult::Type UBTTask_GetPathPoints::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// get the blackboard
	const UBlackboardComponent* OwnerBlackboard = OwnerComp.GetBlackboardComponent();
	// get the controller tied to the tree we're being run by
	AAIController* OwnerController = OwnerComp.GetAIOwner();
	// abort if either don't return valid
	if (!OwnerController || !OwnerBlackboard) {
		return EBTNodeResult::Failed;
	}

	// grab Target that should have PointPath SmartObject tied to it
	ATarget* Target = Cast<ATarget>(OwnerController->GetPawn());
	if (!Target) {
		return EBTNodeResult::Failed;
	}

	// cast PointPath SmartObject from Target's SmartObjectVar
	APointPath* PathRef = Cast<APointPath>(Target->SmartObject);
	// abort if no SmartObject set or SmartObject is not PointPath
	if (!PathRef || PathRef->Locations.Num() < 1) {
		return EBTNodeResult::Succeeded;
	}

	// set Blackboard's move vector comp to the value of one of the Locations in the spline
	OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Vector>("MoveToLocation", PathRef->Locations[index]);

	// increment the locations index
	index++;
	if (index == PathRef->Locations.Num()) {
		index = 0;
	}

	// default to success
	return EBTNodeResult::Succeeded;
}
