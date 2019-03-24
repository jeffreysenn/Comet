// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "LightGameMode.h"
#include "LightPawn.h"

ALightGameMode::ALightGameMode()
{
	// set default pawn class to our flying pawn
	DefaultPawnClass = ALightPawn::StaticClass();
}
