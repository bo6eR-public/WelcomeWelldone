// Copyright © 2025 bo6eR. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "WellPlayerController.generated.h"


UCLASS()
class WELCOMEWELLDONE_API AWellPlayerController : public APlayerController
{
	GENERATED_BODY()

private:
	virtual void ProcessPlayerInput(const float DeltaTime, const bool bGamePaused) override;

protected:
	virtual void BeginPlay() override;

	virtual void NotifyLoadedWorld(FName WorldPackageName, bool bFinalDest) override;

	UFUNCTION(Reliable, Server, WithValidation, SealedEvent)
	void ServerNotifyLoadedWorldWorkaround(FName WorldPackageName);
};

