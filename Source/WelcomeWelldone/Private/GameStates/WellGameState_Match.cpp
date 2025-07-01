// Copyright © 2025 bo6eR. All rights reserved.


#include "GameStates/WellGameState_Match.h"

#include "CommomTypes/WellCommonTypes.h"
#include "CommomTypes/GameInstances/WellGameInstance.h"
#include "Runtime/GameplayMessageSubsystem.h"


void AWellGameState_Match::LeaveSession_Implementation() const
{
	if (const auto GameInstance = CastChecked<UWellGameInstance>(GetGameInstance()))
	{
		GameInstance->DestroySession();
	}
	Multicast_TravelToMenu();
}

void AWellGameState_Match::Multicast_TravelToMenu_Implementation() const
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if (APlayerController* PC = It->Get())
		{
			if (const auto GameInstance = CastChecked<UWellGameInstance>(GetGameInstance()))
			{
				GameInstance->Exit();
			}
		}
	}
}

void AWellGameState_Match::Server_DeathNotify_Implementation() const
{
	NetMulticast_DeathNotify();
}

void AWellGameState_Match::NetMulticast_DeathNotify_Implementation() const
{
	if (UGameplayMessageSubsystem* MessageSubsystem = &UGameplayMessageSubsystem::Get(GetWorld()))
	{
		MessageSubsystem->BroadcastMessage<FGameplayMessage_BoolData>(FGameplayTag::RequestGameplayTag("Message.Death"), FGameplayMessage_BoolData(true));
	}
}
