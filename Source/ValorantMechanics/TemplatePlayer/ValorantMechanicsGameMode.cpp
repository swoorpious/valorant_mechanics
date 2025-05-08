// Copyright Epic Games, Inc. All Rights Reserved.

#include "ValorantMechanicsGameMode.h"
#include "ValorantMechanicsCharacter.h"
#include "UObject/ConstructorHelpers.h"

AValorantMechanicsGameMode::AValorantMechanicsGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
