// Copyright © 2025 bo6eR. All rights reserved.


#include "UserInterface/Widgets/Overlay/WellUserWidget_Overlay.h"
#include "CommomTypes/WellGameplayTags.h"
#include "Runtime/GameplayMessageSubsystem.h"

void UWellUserWidget_Overlay::NativeConstruct()
{
	Super::NativeConstruct();

	UGameplayMessageSubsystem* MessageBusSubsystem = &UGameplayMessageSubsystem::Get(GetWorld());
	if (MessageBusSubsystem != nullptr)
	{
		MessageBusSubsystem->RegisterListener(WellGameplayTags::Message_Modifier, this, &ThisClass::OnModifierStatusChanged);
	}
}
