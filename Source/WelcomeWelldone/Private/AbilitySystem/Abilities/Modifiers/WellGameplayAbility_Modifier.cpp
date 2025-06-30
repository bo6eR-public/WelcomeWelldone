// Copyright © 2025 bo6eR. All rights reserved.


#include "AbilitySystem/Abilities/Modifiers/WellGameplayAbility_Modifier.h"
#include "CommomTypes/WellCommonTypes.h"
#include "Runtime/GameplayMessageSubsystem.h"

void UWellGameplayAbility_Modifier::BroadcastModifierStatusMessage(const EModifierStatus& Status) const
{
	FString ConvertedStatus = "";
	switch (Status)
	{
	case Applied:
		ConvertedStatus = "Applied";
		break;
	case Extended:
		ConvertedStatus = "Extended";
		break;
	case Removed:
		ConvertedStatus = "Removed";
		break;
	default:
		ConvertedStatus = "";
	}

	if (UGameplayMessageSubsystem* MessageSubsystem = &UGameplayMessageSubsystem::Get(GetWorld()))
	{
		MessageSubsystem->BroadcastMessage<FGameplayMessage_ModifierChanged>(FGameplayTag::RequestGameplayTag("Message.Modifier"), FGameplayMessage_ModifierChanged(DisplayName, ConvertedStatus));
	}
}
