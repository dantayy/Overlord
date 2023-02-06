// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "TargetNavMovement.generated.h"

/**
 * 
 */
UCLASS()
class OVERLORD_API UTargetNavMovement : public UPawnMovementComponent
{
	GENERATED_BODY()

private:

	/** Was velocity requested by path following? */
	UPROPERTY(Transient)
	bool bHasRequestedVelocity;

	UPROPERTY(Transient)
	FVector RequestedVelocity;

	/** Was acceleration requested to be always max speed? */
	UPROPERTY(Transient)
	bool bRequestedMoveWithMaxSpeed;

public:
	UTargetNavMovement();
	~UTargetNavMovement();


	//BEGIN UMovementComponent Interface
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void StopActiveMovement() override;
	//END UMovementComponent Interface


	//BEGIN UNavMovementComponent Interface
	virtual void RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed) override;
	virtual bool CanStartPathFollowing() const override;
	virtual bool CanStopPathFollowing() const override;
	virtual float GetPathFollowingBrakingDistance(float MaxSpeed) const override;
	//END UNavMovementComponent Interface
};
