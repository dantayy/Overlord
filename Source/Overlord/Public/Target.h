// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
//#include "GameFramework/NavMovementComponent.h"
//#include "TargetNavMovement.h"
#include "Projectile.h"
#include "Target.generated.h"

// Base class for all targets the player gunship can strike with projectiles
UCLASS()
class OVERLORD_API ATarget : public APawn
{
	GENERATED_BODY()
	
public:	
	// constructor
	ATarget();

	// collision + visual component (make this the root component of the class)
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* VisualMesh;

	// component to allow targets to move
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	//UNavMovementComponent* TargetMovement;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	//UTargetNavMovement* TargetMovement;

	// health pool to be depleted by collisions with projectiles, destroy the target when 0
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target")
	uint8 Health = 1;
	// flag determining if target should count towards level completion (destroy hostiles == good, destroy non-hostiles == not good)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target")
	bool Hostile = true;
	// speed limit to set MaxSpeed for when utilizing more nuanced movement components in child classes (ie humans and vehicles)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	int SpeedLimit = 1000;
	// acceleration limit to set MaxAcceleration for when utilizing more nuanced movement components in child classes (ie humans and vehicles)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	int AccelLimit = 50;
	// behavior tree that can control a target
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	class UBehaviorTree* TargetTree;
	// object in world with custom behaviors assigned to it that the target's bt will run if the controller is made to do so
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	class ASmartObject* SmartObject;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// name of most recent projectile to hit target, used to prevent double-hit events
	FString RecentProjectile = "";

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// function to be triggered when the visual/collision component is collided with
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	// function to be triggered when the visual/collision component is overlapped
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
