// Fill out your copyright notice in the Description page of Project Settings.


#include "Target.h"
#include "Overlord/OverlordGameModeBase.h"

// Sets default values
ATarget::ATarget()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// create the visual mesh, set it to simulate physics, and attach the OnHit function to collisions
	VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	VisualMesh->SetupAttachment(RootComponent);

	// set up movement component
	//TargetMovement = CreateDefaultSubobject<UNavMovementComponent>(TEXT("Movement"));
	// Create an instance of our movement component, and tell it to update our root component.
	//TargetMovement = CreateDefaultSubobject<UTargetNavMovement>(TEXT("Movement"));
	//TargetMovement->UpdatedComponent = RootComponent;

	// set a default mesh of a target
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube"));
	if (CubeVisualAsset.Succeeded())
	{
		VisualMesh->SetStaticMesh(CubeVisualAsset.Object);
		VisualMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	}

	// enable physics and overlap events
	VisualMesh->SetSimulatePhysics(true);
	VisualMesh->SetGenerateOverlapEvents(true);

}

// Called when the game starts or when spawned
void ATarget::BeginPlay()
{
	// this is needed for destroy calls to work
	Super::BeginPlay();

	// set up collision handling
	VisualMesh->OnComponentHit.AddDynamic(this, &ATarget::OnHit);
	VisualMesh->OnComponentBeginOverlap.AddDynamic(this, &ATarget::OnOverlap);

	// add to game mode's hostile tracker if target is hostile and we're in the right game mode
	AOverlordGameModeBase* OverloardGM = Cast<AOverlordGameModeBase>(GetWorld()->GetAuthGameMode());
	if (OverloardGM && Hostile) {
		OverloardGM->HostileTargets.Add(this);
		this->OnDestroyed.AddDynamic(OverloardGM, &AOverlordGameModeBase::HostileDestroyed);
	}
}

// Called every frame
void ATarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATarget::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	FString ProjectileName;
	OtherActor->GetName(ProjectileName);
	if (ProjectileName == RecentProjectile) {
		return;
	}
	RecentProjectile = ProjectileName;
	// check for collision with Projectiles
	if (OtherActor->GetClass()->IsChildOf(AProjectile::StaticClass())) {
		// debug logging Projectile hit
		if (GEngine) {
			// Display a debug message for five seconds
			// The -1 "Key" value argument prevents the message from being updated or refreshed
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Target hit by Projectile"));
		}
		// cast to Projectile
		AProjectile* HitProjectile = static_cast<AProjectile*>(OtherActor);
		if (GEngine) {
			// Display a debug message for five seconds
			// The -1 "Key" value argument prevents the message from being updated or refreshed
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Projectile Damage: %d"), HitProjectile->Damage));
		}
		// reduce health by ammount of damage the Projectile provides
		Health -= FMath::Min(HitProjectile->Damage, Health);
		if (GEngine) {
			// Display a debug message for five seconds
			// The -1 "Key" value argument prevents the message from being updated or refreshed
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("New current health: %d"), Health));
		}
	}
	// destroy this object if it's health has reached 0
	if (Health == 0) {
		// debug logging Target destruction
		if (GEngine) {
			// Display a debug message for five seconds
			// The -1 "Key" value argument prevents the message from being updated or refreshed
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Target destroyed"));
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
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Orange, TEXT("Target hit by Explosion"));
		}
		AExplosion* HitExplosion = static_cast<AExplosion*>(OtherActor);
		if (GEngine) {
			// Display a debug message for five seconds
			// The -1 "Key" value argument prevents the message from being updated or refreshed
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Orange, FString::Printf(TEXT("Explosion Damage: %d"), HitExplosion->Damage));
		}
		Health -= FMath::Min(HitExplosion->Damage, Health);
		if (GEngine) {
			// Display a debug message for five seconds
			// The -1 "Key" value argument prevents the message from being updated or refreshed
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Orange, FString::Printf(TEXT("New current health: %d"), Health));
		}
	}
	// destroy this object if it's health has reached 0
	if (Health == 0) {
		// debug logging Target destruction
		if (GEngine) {
			// Display a debug message for five seconds
			// The -1 "Key" value argument prevents the message from being updated or refreshed
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Orange, TEXT("Target destroyed"));
		}
		// destroy the Target
		Destroy();
	}
}

void ATarget::Fire()
{
	// Attempt to fire a projectile
	if (!EquippedWeapon) {
		// log unsuccessful firing
		if (GEngine) {
			// Display a debug message for five seconds
			// The -1 "Key" value argument prevents the message from being updated or refreshed
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("NO WEAPON EQUIPPED"));
		}
		return;
	}
	else if (!EquippedWeapon->ProjectileClass)
	{
		// log unsuccessful firing
		if (GEngine) {
			// Display a debug message for five seconds
			// The -1 "Key" value argument prevents the message from being updated or refreshed
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("NO PROJECTILE ATTACHED TO WEAPON"));
		}
		return;
	}
	// check to make sure Gunship is in an active world before spawning Projectile
	UWorld* World = GetWorld();
	if (!World) {
		// log unsuccessful firing
		if (GEngine) {
			// Display a debug message for five seconds
			// The -1 "Key" value argument prevents the message from being updated or refreshed
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("TARGET NOT IN ACTIVE WORLD!"));
		}
		return;
	}
	if (EquippedWeapon->MaxAmmo != 0 && EquippedWeapon->CurrentAmmo == 0) {
		// log unsuccessful firing
		if (GEngine) {
			// Display a debug message for five seconds
			// The -1 "Key" value argument prevents the message from being updated or refreshed
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Orange, TEXT("OUT OF AMMO! NEED TO RELOAD!"));
		}
		return;
	}
	// create spawn parameters
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();

	// Spawn the projectile at the muzzle
	AProjectile* Projectile = World->SpawnActor<AProjectile>(EquippedWeapon->ProjectileClass, EquippedWeapon->MuzzleLocation, EquippedWeapon->MuzzleDirection, SpawnParams);

	if (!Projectile)
	{
		// log unsuccessful firing
		if (GEngine) {
			// Display a debug message for five seconds
			// The -1 "Key" value argument prevents the message from being updated or refreshed
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("PROJECTILE NOT SPAWNED FOR UNKNOWN REASONS, ABORTING."));
		}
		return;
	}

	// Decrement weapon's ammo if not unlimitted
	if (EquippedWeapon->MaxAmmo != 0) {
		EquippedWeapon->CurrentAmmo--;
		// begin reload if out of ammo
		if (EquippedWeapon->CurrentAmmo < 1) {
			GetWorldTimerManager().SetTimer(EquippedWeapon->ReloadTimer, FTimerDelegate::CreateLambda([&] { EquippedWeapon->CurrentAmmo = EquippedWeapon->MaxAmmo; }), EquippedWeapon->ReloadTime, false);
		}
	}

	// fire the projectile
	Projectile->FireInDirection(EquippedWeapon->MuzzleDirection.Vector());
	// log successful firing
	if (GEngine) {
		// Display a debug message for five seconds
		// The -1 "Key" value argument prevents the message from being updated or refreshed
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Firing"));
	}
}

int ATarget::GetMaxAmmo()
{
	if (EquippedWeapon) {
		return EquippedWeapon->MaxAmmo;
	}
	return -1;
}

int ATarget::GetCurrentAmmo()
{
	if (EquippedWeapon) {
		return EquippedWeapon->CurrentAmmo;
	}
	return -1;
}

