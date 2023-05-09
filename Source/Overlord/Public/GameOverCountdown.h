// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Overlord/OverlordGameModeBase.h"
#include "GameOverCountdown.generated.h"

UCLASS()
class OVERLORD_API AGameOverCountdown : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameOverCountdown();

	// time until Overlord's gameover is called
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	double CountdownTime = 0.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	FTimerHandle GameOverHandle;
	
	UFUNCTION()
	void ClearTimer(AActor* Timer);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
