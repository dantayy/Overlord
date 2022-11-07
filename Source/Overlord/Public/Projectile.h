// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Explosion.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Projectile.generated.h"

// Base class for objects that can damage Targets
UCLASS()
class OVERLORD_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// constructor
	AProjectile();

	// Sphere collision component
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	USphereComponent* CollisionComponent;

	// Projectile movement component
	UPROPERTY(VisibleAnywhere, Category = Movement)
	UProjectileMovementComponent* ProjectileMovementComponent;

	// Projectile mesh
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	UStaticMeshComponent* ProjectileMeshComponent;

	// Projectile material
	UPROPERTY(VisibleDefaultsOnly, Category = Movement)
	UMaterialInstanceDynamic* ProjectileMaterialInstance;

	// Damage a projectile will do to a target on hit
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile)
	uint8 Damage = 1;

	// Explosion to be emitted on collision, leave empty if no explosion desired
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Explosion)
	TSubclassOf<class AExplosion> ProjectileExplosion;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Function to be triggered when the visual/collision component is collided with
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	// Function that initializes the projectile's velocity in the shoot direction
	void FireInDirection(const FVector& ShootDirection);

};
