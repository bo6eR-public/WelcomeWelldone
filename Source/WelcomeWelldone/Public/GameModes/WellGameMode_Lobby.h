// Copyright © 2025 bo6eR. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "WellGameMode_Lobby.generated.h"


UCLASS()
class WELCOMEWELLDONE_API AWellGameMode_Lobby : public AGameModeBase
{
	GENERATED_BODY()

public:
	AWellGameMode_Lobby();

protected:
	virtual void PostLogin(APlayerController* NewPlayer) override;

};
