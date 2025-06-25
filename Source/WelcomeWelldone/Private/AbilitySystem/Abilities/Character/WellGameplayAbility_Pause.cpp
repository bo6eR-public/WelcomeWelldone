// Copyright © 2025 bo6eR. All rights reserved.


#include "AbilitySystem/Abilities/Character/WellGameplayAbility_Pause.h"
#include "UserInterface/HUDs/WellHUD_Master.h"

UWellGameplayAbility_Pause::UWellGameplayAbility_Pause()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	ActivationPolicy = EAbilityActivationPolicy::Triggered;
}

void UWellGameplayAbility_Pause::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	APlayerController* PlayerController = ActorInfo->PlayerController.Get();
	if (PlayerController && PlayerController->IsLocalController())
	{
		const auto GameHUD = GetGameHUDFromPlayerController(PlayerController);
		check(GameHUD);
		GameHUD->PushToLayers(PauseWidgetClass);

		const FInputModeUIOnly InputMode;
		PlayerController->SetInputMode(InputMode);
		PlayerController->SetShowMouseCursor(true);
	}
	EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}

AWellHUD_Master* UWellGameplayAbility_Pause::GetGameHUDFromPlayerController(const APlayerController* PlayerController) const
{
	return PlayerController ? Cast<AWellHUD_Master>(PlayerController->GetHUD()) : nullptr;
}
