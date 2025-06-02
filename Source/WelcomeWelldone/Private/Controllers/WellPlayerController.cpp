// Copyright © 2025 bo6eR. All rights reserved.


#include "WelcomeWelldone/Public/Controllers/WellPlayerController.h"
#include "AbilitySystem/AbilitySystemComponents/WellAbilitySystemComponent.h"
#include "Characters/WellCharacter.h"


void AWellPlayerController::ProcessPlayerInput(const float DeltaTime, const bool bGamePaused)
{
	Super::ProcessPlayerInput(DeltaTime, bGamePaused);
	AWellCharacter* AbilityCharacter = Cast<AWellCharacter>(GetCharacter());
	if (AbilityCharacter != nullptr)
	{
		if (const auto AbilitySystemComponent = AbilityCharacter->GetWellAbilitySystemComponent())
		{
			AbilitySystemComponent->ProcessAbilityInput(DeltaTime, bGamePaused);
		}
	}
}
