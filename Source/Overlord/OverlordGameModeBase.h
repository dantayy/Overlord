// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Target.h"
#include "OverlordGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class OVERLORD_API AOverlordGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

	virtual void StartPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> Levels;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 LevelIterator = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<ATarget*> HostileTargets;

	/** Remove the current menu widget and create a new one from the specified class, if provided. */
	UFUNCTION(BlueprintCallable, Category = "UMG Game")
	void ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass);

protected:

	/** Called when the game starts. */
	virtual void BeginPlay() override;

	/** Called when a member of HostileTargets is destroyed */
	UFUNCTION()
	void HostileDestroyed(AActor* DestroyedActor);

	/** The widget class we will use as our menu when the game starts. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG Game")
	TSubclassOf<UUserWidget> StartingWidgetClass;

	/** The widget instance that we are using as our menu. */
	UPROPERTY()
	UUserWidget* CurrentWidget;
	
};
