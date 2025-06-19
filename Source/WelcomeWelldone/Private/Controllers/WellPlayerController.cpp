// Copyright © 2025 bo6eR. All rights reserved.


#include "WelcomeWelldone/Public/Controllers/WellPlayerController.h"
#include "AbilitySystem/AbilitySystemComponents/WellAbilitySystemComponent.h"
#include "Characters/WellCharacter.h"
#include "UserInterface/HUDs/WellHUD_MainGame.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(WellPlayerController)

void AWellPlayerController::ProcessPlayerInput(const float DeltaTime, const bool bGamePaused)
{
	Super::ProcessPlayerInput(DeltaTime, bGamePaused);
	
	if (AWellCharacter* AbilityCharacter = Cast<AWellCharacter>(GetCharacter()); AbilityCharacter != nullptr)
	{
		if (const auto AbilitySystemComponent = AbilityCharacter->GetWellAbilitySystemComponent())
		{
			AbilitySystemComponent->ProcessAbilityInput(DeltaTime, bGamePaused);
		}
	}
}

void AWellPlayerController::BeginPlay()
{
	Super::BeginPlay();

	PlayerCameraManager->ViewPitchMin = -75.f;
	PlayerCameraManager->ViewPitchMax = 75.f;
	
	if (IsLocalPlayerController())
	{
		AWellHUD_MainGame* MainGameHUD = CastChecked<AWellHUD_MainGame>(GetHUD());
		if (MainGameHUD != nullptr)
		{
			MainGameHUD->InitializeHUD();
		}
	}
}
