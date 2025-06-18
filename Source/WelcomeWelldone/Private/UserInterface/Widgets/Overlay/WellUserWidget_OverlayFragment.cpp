// Copyright © 2025 bo6eR. All rights reserved.


#include "UserInterface/Widgets/Overlay/WellUserWidget_OverlayFragment.h"
#include "CommomTypes/WellGameplayTags.h"
#include "Runtime/GameplayMessageSubsystem.h"

void UWellUserWidget_OverlayFragment::NativeConstruct()
{
	Super::NativeConstruct();

	UGameplayMessageSubsystem* MessageBusSubsystem = &UGameplayMessageSubsystem::Get(GetWorld());
	if (MessageBusSubsystem != nullptr)
	{
		MessageBusSubsystem->RegisterListener(WellGameplayTags::Message_Attribute, this, &ThisClass::OnAttributeChanged);
		MessageBusSubsystem->RegisterListener(WellGameplayTags::Message_Instance_Weapon, this, &ThisClass::OnWeaponDataChanged);
	}
}
