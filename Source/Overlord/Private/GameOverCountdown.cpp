// Fill out your copyright notice in the Description page of Project Settings.


#include "GameOverCountdown.h"

// Sets default values
AGameOverCountdown::AGameOverCountdown()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AGameOverCountdown::BeginPlay()
{
	Super::BeginPlay();
	
	// verify valid time has been set before setting timer
	if (CountdownTime > 0) {
		// start timer that'll trigger Overlord's game over when 0
		GetWorldTimerManager().SetTimer(
			GameOverHandle,
			FTimerDelegate::CreateLambda(
				[&] {
					AOverlordGameModeBase* OverloardGM = Cast<AOverlordGameModeBase>(GetWorld()->GetAuthGameMode());
					// make sure we're in the right game mode
					if (OverloardGM) {
						OverloardGM->GameOver();
					}
				}),
			CountdownTime,
					false);
	}
	// clear the timer when this is destroyed (when the level is ended)
	this->OnEndPlay.AddDynamic(this, &AGameOverCountdown::ClearTimer);
}

void AGameOverCountdown::ClearTimer(AActor* Timer, EEndPlayReason::Type EndPlayReason)
{
	GetWorldTimerManager().ClearTimer(GameOverHandle);
}

// Called every frame
void AGameOverCountdown::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CountdownTime -= DeltaTime;
}

