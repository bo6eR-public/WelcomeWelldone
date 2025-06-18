// Copyright © 2025 bo6eR. All rights reserved.


#include "UserInterface/HUDs/WellHUD_MainGame.h"
#include "Blueprint/UserWidget.h"

void AWellHUD_MainGame::InitializeHUD()
{
	checkf(PlayerOverlayClass, TEXT("Player overlay class is not valid!"));
	
	const APlayerController* PlayerController = GetOwningPlayerController();
	if (PlayerController && PlayerController->IsLocalPlayerController())
	{
		PlayerOverlay = CreateWidget<UUserWidget>(GetWorld(), PlayerOverlayClass, FName(TEXT("Player Overlay Widget")));
		if (PlayerOverlay != nullptr)
		{
			PlayerOverlay->AddToViewport();
		}
	}
}

void AWellHUD_MainGame::UninitializeHUD()
{
	if (PlayerOverlay != nullptr)
	{
		PlayerOverlay->RemoveFromParent();
	}
}
