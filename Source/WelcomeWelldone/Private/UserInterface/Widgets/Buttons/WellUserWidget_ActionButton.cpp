// Copyright © 2025 bo6eR. All rights reserved.


#include "UserInterface/Widgets/Buttons/WellUserWidget_ActionButton.h"
#include "CommomTypes/WellGameplayTags.h"
#include "Runtime/GameplayMessageSubsystem.h"

void UWellUserWidget_ActionButton::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (UGameplayMessageSubsystem* MessageBusSubsystem = &UGameplayMessageSubsystem::Get(GetWorld()))
	{
		/* Calls when client-server starts the session search */
		MessageBusSubsystem->RegisterListener<FGameplayMessage_BoolData>(WellGameplayTags::Message_Sessions_Finding, this, &ThisClass::OnFindSession);
	}
}
