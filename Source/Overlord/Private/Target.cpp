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
	VisualMesh->OnComponentHit.AddDynamic(this, &ATarget::OnHit);

	// set a default mesh of a target
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube"));
	if (CubeVisualAsset.Succeeded())
	{
		VisualMesh->SetStaticMesh(CubeVisualAsset.Object);
		VisualMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	}
}

// Called when the game starts or when spawned
void ATarget::BeginPlay()
{
	// this is needed for destroy calls to work
	Super::BeginPlay();
}

// Called every frame
void ATarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATarget::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	// check for collision with Projectiles
	if (OtherActor->GetClass()->GetName() == "Projectile") {
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
		// destroy this object if it's health has reached 0
		if (Health == 0) {
			// debug logging Target destruction
			if (GEngine) {
				// Display a debug message for five seconds
				// The -1 "Key" value argument prevents the message from being updated or refreshed
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Target destroyed by Projectile"));
			}
			// destroy the Target
			Destroy();
		}
	}
}

