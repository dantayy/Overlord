// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "OverlordGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class OVERLORD_API AOverlordGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	// constructor
	AOverlordGameStateBase();

	// hostiles that exist in the level this game state is attached to
	// set this with the game mode I think?
	// have target actors grab the active game mode, cast to OverlordGameModeBase
	// if target is hostile, call a gamemode function that will set this gamestate var
	// then, if hostile targets are destroyed, repeat the process but with a decrementor
	// if, after a decrement, this state lists 0 hostiles, have the gamemode(?) change the level
	// game mode tracks the level count, level count determines which level is up next, including a game over when no more "regular" levels are left
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 HostileCount;
	
};
