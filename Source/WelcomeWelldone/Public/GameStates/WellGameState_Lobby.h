// Copyright © 2025 bo6eR. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "WellGameState_Lobby.generated.h"


UCLASS()
class WELCOMEWELLDONE_API AWellGameState_Lobby : public AGameStateBase
{
	GENERATED_BODY()

public:
	AWellGameState_Lobby();

public:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void StartCountDown();

private:
	UFUNCTION()
	void OnRep_Countdown();

private:
	UPROPERTY()
	FTimerHandle CountdownTimerHandle;

	UPROPERTY(ReplicatedUsing=OnRep_Countdown, EditDefaultsOnly, Category=Time, meta=(AllowPrivateAccess=true))
	/* Time to begin the main game (please, set the value + 1, for example: if you want 10, then set 11) */
	int32 Countdown = 10;
};
