// Copyright © 2025 bo6eR. All rights reserved.


#include "GameModes/WellGameMode_Lobby.h"
#include "GameStates/WellGameState_Lobby.h"

AWellGameMode_Lobby::AWellGameMode_Lobby()
{
	GameStateClass = AWellGameState_Lobby::StaticClass();
	bUseSeamlessTravel = true;
}

void AWellGameMode_Lobby::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (GameState->PlayerArray.Num() == 2)
	{
		if (AWellGameState_Lobby* LobbyGameState = Cast<AWellGameState_Lobby>(GameState))
		{
			LobbyGameState->StartCountDown();
		}
	}
}
