// Copyright © 2025 bo6eR. All rights reserved.


#include "WelcomeWelldone/Public/GameModes/WellGameMode.h"
#include "Characters/WellCharacter.h"
#include "Controllers/WellPlayerController.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(WellGameMode)

AWellGameMode::AWellGameMode()
{
	PlayerControllerClass = AWellPlayerController::StaticClass();
	DefaultPawnClass = AWellCharacter::StaticClass();

	bUseSeamlessTravel = true;
}
