// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TargetAIController.generated.h"

/**
 * AI Controller to be used with Targets, set blueprint to BP_Target's AI Controller Class to use
 */
UCLASS()
class OVERLORD_API ATargetAIController : public AAIController
{
	GENERATED_BODY()
	
public:

	// constructor
	ATargetAIController(const FObjectInitializer& ObjectInitializer);

	// run when loaded into world
	virtual void BeginPlay() override;

	// run when pawn possessed
	virtual void OnPossess(APawn* InPawn) override;

	// Target this controller is attached to
	class ATarget* Agent;

	// the tree this controller is running
	UPROPERTY(transient)
	class UBehaviorTreeComponent* TargetBehaviorTree;

	// the blackboard this controller is running with
	UPROPERTY(transient)
	class UBlackboardComponent* TargetBlackboard;

	// Blackboard Key IDs
	uint8 ObjectiveKeyID;
	uint8 LocationKeyID;
};
