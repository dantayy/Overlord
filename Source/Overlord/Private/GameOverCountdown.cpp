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
	
	if (CountdownTime > 0) {
		FTimerHandle GameOverHandle;
		GetWorldTimerManager().SetTimer(
			GameOverHandle,
			FTimerDelegate::CreateLambda(
				[&] {
					// start timer that'll trigger Overlord's gamer over when 0
					AOverlordGameModeBase* OverloardGM = Cast<AOverlordGameModeBase>(GetWorld()->GetAuthGameMode());
					// make sure we're in the right game mode and that the timer has been set up beforehand
					if (OverloardGM) {
						OverloardGM->GameOver();
					}
				}),
			CountdownTime,
					false);
	}
}

// Called every frame
void AGameOverCountdown::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CountdownTime -= DeltaTime;
}

