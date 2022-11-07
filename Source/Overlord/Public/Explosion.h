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
	USphereComponent* CollisionComponent;

	// radius of explosion collision sphere
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 Radius = 10;

	// ammount of damage dealt to targets in collision radius
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 Damage = 1;

	// duration of explosion particle effects
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 Duration = 3;

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

};
