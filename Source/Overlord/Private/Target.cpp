// Fill out your copyright notice in the Description page of Project Settings.


#include "Target.h"

// Sets default values
ATarget::ATarget()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// create the visual mesh, set it to simulate physics, and attach the OnHit function to collisions
	VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	VisualMesh->SetupAttachment(RootComponent);

	// set a default mesh of a target
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube"));
	if (CubeVisualAsset.Succeeded())
	{
		VisualMesh->SetStaticMesh(CubeVisualAsset.Object);
		VisualMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	}

	VisualMesh->SetSimulatePhysics(true);
	VisualMesh->SetGenerateOverlapEvents(true);
}

// Called when the game starts or when spawned
void ATarget::BeginPlay()
{
	// this is needed for destroy calls to work
	Super::BeginPlay();

	VisualMesh->OnComponentHit.AddDynamic(this, &ATarget::OnHit);
	VisualMesh->OnComponentBeginOverlap.AddDynamic(this, &ATarget::OnOverlap);
}

// Called every frame
void ATarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATarget::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	// check for collision with Projectiles
	if (OtherActor->GetClass()->IsChildOf(AProjectile::StaticClass())) {
		// debug logging Projectile hit
		if (GEngine) {
			// Display a debug message for five seconds
			// The -1 "Key" value argument prevents the message from being updated or refreshed
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Target hit by Projectile"));
		}
		// cast to Projectile
		AProjectile* HitProjectile = static_cast<AProjectile*>(OtherActor);
		// reduce health by ammount of damage the Projectile provides
		Health -= HitProjectile->Damage;
	}
	//else if (OtherActor->GetClass()->IsChildOf(AExplosion::StaticClass())) {
	//	// debug logging Explosion hit
	//	if (GEngine) {
	//		// Display a debug message for five seconds
	//		// The -1 "Key" value argument prevents the message from being updated or refreshed
	//		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Target hit by Explosion"));
	//	}
	//	AExplosion* HitExplosion = static_cast<AExplosion*>(OtherActor);
	//	Health -= HitExplosion->Damage * (GetDistanceTo(HitExplosion) / HitExplosion->Radius);
	//}
	// destroy this object if it's health has reached 0
	if (Health <= 0) {
		// debug logging Target destruction
		if (GEngine) {
			// Display a debug message for five seconds
			// The -1 "Key" value argument prevents the message from being updated or refreshed
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Target destroyed"));
		}
		// destroy the Target
		Destroy();
	}
}

void ATarget::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->GetClass()->IsChildOf(AExplosion::StaticClass())) {
		// debug logging Explosion hit
		if (GEngine) {
			// Display a debug message for five seconds
			// The -1 "Key" value argument prevents the message from being updated or refreshed
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Target hit by Explosion"));
		}
		AExplosion* HitExplosion = static_cast<AExplosion*>(OtherActor);
		float DistanceFromExplosion = GetDistanceTo(HitExplosion);
		if (GEngine) {
			// Display a debug message for five seconds
			// The -1 "Key" value argument prevents the message from being updated or refreshed
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Purple, FString::Printf(TEXT("Distance from Explosion: %f"), DistanceFromExplosion));
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Purple, FString::Printf(TEXT("Explosion radius: %f"), HitExplosion->Radius));
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Purple, FString::Printf(TEXT("Proportional Damage: %f"), HitExplosion->Damage * (DistanceFromExplosion / HitExplosion->Radius)));
		}
		Health -= HitExplosion->Damage * (GetDistanceTo(HitExplosion) / HitExplosion->Radius);
		if (GEngine) {
			// Display a debug message for five seconds
			// The -1 "Key" value argument prevents the message from being updated or refreshed
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Purple, FString::Printf(TEXT("New current health: %f"), Health));
		}
	}
	if (GEngine) {
		// Display a debug message for five seconds
		// The -1 "Key" value argument prevents the message from being updated or refreshed
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Purple, FString::Printf(TEXT("New current health: %f"), Health));
	}
	// destroy this object if it's health has reached 0
	if (Health == 0) {
		// debug logging Target destruction
		if (GEngine) {
			// Display a debug message for five seconds
			// The -1 "Key" value argument prevents the message from being updated or refreshed
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Target destroyed"));
		}
		// destroy the Target
		Destroy();
	}
}

