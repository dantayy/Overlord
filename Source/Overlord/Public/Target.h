// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
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

	// health pool to be depleted by collisions with projectiles, destroy the target when 0
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target")
	uint8 Health = 1;
	// flag determining if target should count towards level completion (destroy hostiles == good, destroy non-hostiles == not good)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target")
	bool Hostile = true;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// function to be triggered when the visual/collision component is collided with
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

};
