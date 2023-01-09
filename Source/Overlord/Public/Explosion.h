// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Explosion.generated.h"

UCLASS()
class OVERLORD_API AExplosion : public AActor
{
	GENERATED_BODY()
	
public:

	// Sets default values for this actor's properties
	AExplosion();

	// particle effects for explosion
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Particles)
	UParticleSystemComponent* ExplosionEffect;

	// collision component used to trigger damage, set up via set radius value
	UPROPERTY(VisibleDefaultsOnly, Category = Explosion)
	USphereComponent* CollisionComponent;

	// radius of explosion collision sphere
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Radius = 400;

	// ammount of damage dealt to targets in collision radius
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 Damage = 1;

	// duration of explosion particle effects
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 Duration = 4;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// timer manager
	FTimerHandle ExplosionTimer;

	// callback to destroy this explosion
	UFUNCTION()
	void EndExplosion();

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
