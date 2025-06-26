// Copyright © 2025 bo6eR. All rights reserved.


#include "GameStates/WellGameState_Match.h"
#include "CommomTypes/GameInstances/WellGameInstance.h"


void AWellGameState_Match::LeaveSession() const
{
	Multicast_TravelToMenu_Implementation();
	
	if (PlayerArray.Num())
	{
		if (const auto GameInstance = CastChecked<UWellGameInstance>(GetGameInstance()))
		{
			GameInstance->DestroySession();

			//GameInstance->ServerTravel("/Game/Levels/Maps/LV_Menu");
			GetWorld()->ServerTravel("/Game/Levels/Maps/LV_Menu?listen", true);
		}
	}
}

void AWellGameState_Match::Multicast_TravelToMenu_Implementation() const
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC && !PC->HasAuthority())
	{
		PC->ClientTravel("/Game/Levels/Maps/LV_Menu", TRAVEL_Absolute);
	}
}
