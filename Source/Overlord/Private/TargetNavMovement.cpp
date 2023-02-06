// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetNavMovement.h"

UTargetNavMovement::UTargetNavMovement()
{
}

UTargetNavMovement::~UTargetNavMovement()
{
}

void UTargetNavMovement::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Make sure that everything is still valid, and that we are allowed to move.
	if (!PawnOwner || !UpdatedComponent || ShouldSkipUpdate(DeltaTime))
	{
		return;
	}

	// Get (and then clear) the movement vector that we set in ACollidingPawn::Tick
	//FVector DesiredMovementThisFrame = ConsumeInputVector().GetClampedToMaxSize(1.0f) * DeltaTime * 150.0f;


	if (RequestedVelocity == FVector::ZeroVector)
	{
		return;
	}
	FVector DesiredMovementThisFrame = RequestedVelocity.GetClampedToMaxSize(1.0f) * DeltaTime * 150.0f;

	if (!DesiredMovementThisFrame.IsNearlyZero())
	{
		FHitResult Hit;
		SafeMoveUpdatedComponent(DesiredMovementThisFrame, UpdatedComponent->GetComponentRotation(), true, Hit);

		// If we bumped into something, try to slide along it
		if (Hit.IsValidBlockingHit())
		{
			SlideAlongSurface(DesiredMovementThisFrame, 1.f - Hit.Time, Hit.Normal, Hit);
		}
	}
}

void UTargetNavMovement::StopActiveMovement()
{
	Super::StopActiveMovement();

	bHasRequestedVelocity = false;
	RequestedVelocity = FVector::ZeroVector;
}

void UTargetNavMovement::RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed)
{
	if (MoveVelocity.SizeSquared() < KINDA_SMALL_NUMBER)
	{
		return;
	}

	if (IsFalling())
	{
		//	const FVector FallVelocity = MoveVelocity.GetClampedToMaxSize(GetMaxSpeed());
		//	PerformAirControlForPathFollowing(FallVelocity, FallVelocity.Z);
		//	return;
			//NOT YET

	}

	RequestedVelocity = MoveVelocity;
	bHasRequestedVelocity = true;
	bRequestedMoveWithMaxSpeed = bForceMaxSpeed;
	RequestedVelocity.Z = 0.0f;

	//if (IsMovingOnGround())
	//{
	//	RequestedVelocity.Z = 0.0f;
	//}
}

bool UTargetNavMovement::CanStartPathFollowing() const
{
	//if (!HasValidData() || HasAnimRootMotion())
	//{
	//	return false;
	//}

	//if (CharacterOwner)
	//{
	//	if (CharacterOwner->GetRootComponent() && CharacterOwner->GetRootComponent()->IsSimulatingPhysics())
	//	{
	//		return false;
	//	}
	//	else if (CharacterOwner->IsMatineeControlled())
	//	{
	//		return false;
	//	}
	//}

	return Super::CanStartPathFollowing();
}

bool UTargetNavMovement::CanStopPathFollowing() const
{
	return !IsFalling();
}

float UTargetNavMovement::GetPathFollowingBrakingDistance(float MaxSpeed) const
{
	if (bUseFixedBrakingDistanceForPaths)
	{
		return FixedPathBrakingDistance;
	}

	//const float BrakingDeceleration = BrakingDecelerationWalking * (bUseSeparateBrakingFriction ? BrakingFriction : GroundFriction);
	const float BrakingDeceleration = 16384.0000;

	// character won't be able to stop with negative or nearly zero deceleration, use MaxSpeed for path length calculations
	const float BrakingDistance = (BrakingDeceleration < SMALL_NUMBER) ? MaxSpeed : (FMath::Square(MaxSpeed) / BrakingDeceleration);
	return BrakingDistance;
}
