// MIT License - Copyright (c) 2023 João Casimiro

#include "MastermindGameMode.h"
#include "UObject/ConstructorHelpers.h"

AMastermindGameMode::AMastermindGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/BPs/BP_LoaderPawn"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
