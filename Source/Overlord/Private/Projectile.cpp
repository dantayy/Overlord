// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// set up root component
	if (!RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSceneComponent"));
	}
	// set up collision component
	if (!CollisionComponent)
	{
		// Use a sphere as a simple collision representation
		CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
		// Set the sphere's collision profile name to "Projectile"
		CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));
		// Event called when component hits something
		CollisionComponent->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
		// Set the sphere's initial collision radius
		CollisionComponent->InitSphereRadius(15.0);
		// Set the root component to be the collision component
		RootComponent = CollisionComponent;
	}
	// set up projectile movement component
	if (!ProjectileMovementComponent) {
		// Use this component ot drive this projectile's movement
		ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
		ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
		ProjectileMovementComponent->InitialSpeed = 3000.0f;
		ProjectileMovementComponent->MaxSpeed = 3000.0f;
		ProjectileMovementComponent->bRotationFollowsVelocity = true;
		ProjectileMovementComponent->bShouldBounce = true;
		ProjectileMovementComponent->Bounciness = 0.3f;
		ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
	}
	// set up mesh component
	if (!ProjectileMeshComponent)
	{
		ProjectileMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMeshComponent"));
		static ConstructorHelpers::FObjectFinder<UStaticMesh> ProjectileMesh(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
		if (ProjectileMesh.Succeeded())
		{
			ProjectileMeshComponent->SetStaticMesh(ProjectileMesh.Object);
		}

	}

	// set up material
	static ConstructorHelpers::FObjectFinder<UMaterial> ProjectileMaterial(TEXT("'/Game/StarterContent/Materials/M_Metal_Steel.M_Metal_Steel'"));
	if (ProjectileMaterial.Succeeded())
	{
		ProjectileMaterialInstance = UMaterialInstanceDynamic::Create(ProjectileMaterial.Object, ProjectileMeshComponent);
	}

	// attach material to mesh, scale mesh, and set mesh as root component
	ProjectileMeshComponent->SetMaterial(0, ProjectileMaterialInstance);
	ProjectileMeshComponent->SetRelativeScale3D(FVector(0.1f, 0.1f, 0.1f));
	ProjectileMeshComponent->SetupAttachment(RootComponent);

	InitialLifeSpan = 3.0f;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	// this is needed for destroy calls to work
	Super::BeginPlay();	
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	// add impulse to physicis simulating objects
	if (OtherActor != this && OtherComponent->IsSimulatingPhysics())
	{
		OtherComponent->AddImpulseAtLocation(ProjectileMovementComponent->Velocity * 100.0f, Hit.ImpactPoint);
	}
	// destroy this projectile
	Destroy();
}

void AProjectile::FireInDirection(const FVector& ShootDirection)
{
	// set the velocity based on the passed direction
	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}

