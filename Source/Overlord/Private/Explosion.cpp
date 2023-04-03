// Fill out your copyright notice in the Description page of Project Settings.


#include "Explosion.h"

AExplosion::AExplosion()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// create the sphere component
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	// Set the sphere's collision profile name to "Projectile"
	CollisionComponent->SetCollisionProfileName(TEXT("Explosion"), true);
	// set collisions to query only
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	// Set sphere's radius equal to the member var
	CollisionComponent->InitSphereRadius(Radius);
	// enable overlap events for collision component
	CollisionComponent->SetGenerateOverlapEvents(true);
	// ignore other projectiles and explosions
	CollisionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Ignore);
	CollisionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Ignore);
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

	// set up collision handling
	//CollisionComponent->OnComponentHit.AddDynamic(this, &AExplosion::OnHit);
	//CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AExplosion::OnOverlap);
	
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

void AExplosion::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{	
	//// debug logging Explosion hit
	//if (GEngine) {
	//	// Display a debug message for five seconds
	//	// The -1 "Key" value argument prevents the message from being updated or refreshed
	//	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Explosion Hit!"));
	//}
	////EndExplosion();
}

void AExplosion::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//// debug logging Explosion hit
	//if (GEngine) {
	//	// Display a debug message for five seconds
	//	// The -1 "Key" value argument prevents the message from being updated or refreshed
	//	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Explosion overlap!"));
	//}
	////EndExplosion();
}

