// Copyright © 2025 bo6eR. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "WellGameState_Match.generated.h"


UCLASS()
class WELCOMEWELLDONE_API AWellGameState_Match : public AGameStateBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category=Sessions)
	void LeaveSession() const;

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_TravelToMenu() const;
};
