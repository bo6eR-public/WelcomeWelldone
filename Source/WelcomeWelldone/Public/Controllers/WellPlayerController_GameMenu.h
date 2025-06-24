// Copyright © 2025 bo6eR. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "WellPlayerController_GameMenu.generated.h"


UCLASS()
class WELCOMEWELLDONE_API AWellPlayerController_GameMenu : public APlayerController
{
	GENERATED_BODY()

	virtual void NotifyLoadedWorld(FName WorldPackageName, bool bFinalDest) override;

	UFUNCTION(Reliable, Server, WithValidation, SealedEvent)
	void ServerNotifyLoadedWorldWorkaround(FName WorldPackageName);
	
};
