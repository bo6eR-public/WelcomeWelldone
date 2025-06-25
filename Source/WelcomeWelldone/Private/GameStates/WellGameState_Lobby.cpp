// Copyright © 2025 bo6eR. All rights reserved.


#include "GameStates/WellGameState_Lobby.h"
#include "CommomTypes/WellCommonTypes.h"
#include "CommomTypes/GameInstances/WellGameInstance.h"
#include "Net/UnrealNetwork.h"
#include "Runtime/GameplayMessageSubsystem.h"


AWellGameState_Lobby::AWellGameState_Lobby()
{
	bReplicates = true;
}

void AWellGameState_Lobby::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, Countdown);
}

void AWellGameState_Lobby::StartCountDown()
{
	if (const UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(CountdownTimerHandle, FTimerDelegate::CreateLambda([this]()
		{
			Countdown -= 1;
			if (!Countdown)
			{
				UWellGameInstance* GameInstance = CastChecked<UWellGameInstance>(GetGameInstance());
				GameInstance->ServerTravel(TEXT("/Game/Levels/Maps/LV_Master"));

				GetWorld()->GetTimerManager().ClearTimer(CountdownTimerHandle);
				return;
			}
			/* It is for listen server */
			if (UGameplayMessageSubsystem* MessageBusSubsystem = &UGameplayMessageSubsystem::Get(GetWorld()))
			{
				MessageBusSubsystem->BroadcastMessage<FGameplayMessage_IntData>(FGameplayTag::RequestGameplayTag("Message.Countdown"),
					FGameplayMessage_IntData(Countdown));
			}
		}), 1.f, true, 0.f);
	}
}

void AWellGameState_Lobby::OnRep_Countdown()
{
	if (Countdown) /* notify clients */ 
	{
		if (UGameplayMessageSubsystem* MessageBusSubsystem = &UGameplayMessageSubsystem::Get(GetWorld()))
		{
			MessageBusSubsystem->BroadcastMessage<FGameplayMessage_IntData>(FGameplayTag::RequestGameplayTag("Message.Countdown"),
				FGameplayMessage_IntData(Countdown));
		}
	}
}
