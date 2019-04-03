// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "CometGameMode.h"
#include "CometPawn.h"

ACometGameMode::ACometGameMode()
{
	// set default pawn class to our flying pawn
	DefaultPawnClass = ACometPawn::StaticClass();
}
