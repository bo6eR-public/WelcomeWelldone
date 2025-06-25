// Copyright © 2025 bo6eR. All rights reserved.


#include "UserInterface/Widgets/Lobby/WellUserWidget_Lobby.h"
#include "CommomTypes/WellGameplayTags.h"
#include "Runtime/GameplayMessageSubsystem.h"

void UWellUserWidget_Lobby::NativeConstruct()
{
	Super::NativeConstruct();

	UGameplayMessageSubsystem* MessageBusSubsystem = &UGameplayMessageSubsystem::Get(GetWorld());
	if (MessageBusSubsystem != nullptr)
	{
		/* Timer to begin the main game */
		MessageBusSubsystem->RegisterListener(WellGameplayTags::Message_Countdown, this, &ThisClass::OnCountdownChanged);
	}
}
