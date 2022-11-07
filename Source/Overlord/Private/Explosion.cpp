// Fill out your copyright notice in the Description page of Project Settings.


#include "Explosion.h"

AExplosion::AExplosion()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// create the sphere component
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	// Set the sphere's collision profile name to "Projectile"
	CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Explosion"));
	// Set sphere's radius equal to the member var
	CollisionComponent->InitSphereRadius(Radius);
	// set collision component as root
	RootComponent = CollisionComponent;
	// start the particle effects
	ExplosionEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComponent"));
	ExplosionEffect->SetupAttachment(RootComponent);
	if (ExplosionEffect) {
		ExplosionEffect->Activate();
	}
}

// Called when the game starts or when spawned
void AExplosion::BeginPlay()
{
	Super::BeginPlay();
	// make a timer that will destroy this explosion after the ammount of time specified by the member var passes
	GetWorldTimerManager().SetTimer(ExplosionTimer, this, &AExplosion::EndExplosion, Duration, false);
}

void AExplosion::EndExplosion()
{
	Destroy();
}

// Called every frame
void AExplosion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

