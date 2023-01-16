// Copyright Epic Games, Inc. All Rights Reserved.


#include "OverlordGameModeBase.h"

void AOverlordGameModeBase::StartPlay()
{
	Super::StartPlay();
}

void AOverlordGameModeBase::ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass)
{
    if (CurrentWidget != nullptr)
    {
        CurrentWidget->RemoveFromViewport();
        CurrentWidget = nullptr;
    }
    if (NewWidgetClass != nullptr)
    {
        CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), NewWidgetClass);
        if (CurrentWidget != nullptr)
        {
            CurrentWidget->AddToViewport();
        }
    }
}

void AOverlordGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	check(GEngine != nullptr);

	// Display a debug message for five seconds
	// The -1 "Key" value argument prevents the message from being updated or refreshed
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Hello World, this is Overlord!"));

	ChangeMenuWidget(StartingWidgetClass);

    // determine which level we are starting on
    LevelIterator = 0;
    for (UINT8 i = 0; i < Levels.Num(); i++) {
        FName LevelName = *GetLevel()->GetOuter()->GetName();
        if (Levels[i] == LevelName) {
            LevelIterator = i;
            break;
        }
    }

    // grab all targets in level
    TArray<AActor*> TargetsInLevel;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATarget::StaticClass(), TargetsInLevel);

    // iterate through targets to find hostiles
    for (AActor* Target : TargetsInLevel) {
        // cast back to target
        ATarget* CastedTarget = Cast<ATarget>(Target);
        // if hostile, add to member array and bind OnDestroy to our custom handler
        if (CastedTarget->Hostile) {
            HostileTargets.Add(CastedTarget);
            CastedTarget->OnDestroyed.AddDynamic(this, &AOverlordGameModeBase::HostileDestroyed);
        }
    }
}

void AOverlordGameModeBase::HostileDestroyed(AActor* DestroyedActor)
{
    // cast back to target
    ATarget* CastedTarget = Cast<ATarget>(DestroyedActor);
    // remove from array
    HostileTargets.Remove(CastedTarget);
    // check if level "complete"
    if (HostileTargets.IsEmpty()) {
        // iterate level iterator
        LevelIterator++;
        // temp implementation loops back to first level if at end of array
        if (LevelIterator >= Levels.Num()) {
            LevelIterator = 0;
            // handle edge case of no stored levels
            if (LevelIterator == Levels.Num()) {
                return;
            }
        }
        // open level with given name at index pointed to by level iterator
        UGameplayStatics::OpenLevel(GetWorld(), Levels[LevelIterator]);
    }
}
