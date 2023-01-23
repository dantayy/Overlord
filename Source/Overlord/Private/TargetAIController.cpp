// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetAIController.h"
#include "SmartObject.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Target.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

ATargetAIController::ATargetAIController(const FObjectInitializer& ObjectInitializer)
{
	// initialize tree & blackboard
	TargetBlackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Target Blackboard Component"));
	TargetBehaviorTree = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("Target Behavior Tree Component"));
}

void ATargetAIController::BeginPlay()
{
	// run parent's functionality
	Super::BeginPlay();

	// set up the agent pointer
	if (!Agent) {
		ATarget* Target = Cast<ATarget>(GetPawn());
		if (!Target) {
			return;
		}
		Agent = Target;
	}

	// set dynamic functionality to Target's assigned SmartObject subtree, if they have one
	if (Agent->SmartObject) {
		FGameplayTag SubTag;
		TargetBehaviorTree->SetDynamicSubtree(SubTag, Agent->SmartObject->SubTree);
	}
}

void ATargetAIController::OnPossess(APawn* InPawn)
{
	// run parent's functionality
	Super::OnPossess(InPawn);

	// grab passed target
	ATarget* Target = Cast<ATarget>(InPawn);
	// make sure target is valid and has a tree
	if (Target != nullptr && Target->TargetTree != nullptr) {
		// Initialize Blackboard
		TargetBlackboard->InitializeBlackboard(*Target->TargetTree->BlackboardAsset);

		// Set blackboard key IDs
		ObjectiveKeyID = TargetBlackboard->GetKeyID("ObjActor");
		LocationKeyID = TargetBlackboard->GetKeyID("MoveToLocation");

		// start the behavior tree
		TargetBehaviorTree->StartTree(*Target->TargetTree);
	}
}
