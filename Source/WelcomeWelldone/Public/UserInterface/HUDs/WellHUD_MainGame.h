// Copyright © 2025 bo6eR. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "WellHUD_MainGame.generated.h"


UCLASS()
class WELCOMEWELLDONE_API AWellHUD_MainGame : public AHUD
{
	GENERATED_BODY()

public:
	virtual void InitializeHUD();
	virtual void UninitializeHUD();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Overlay)
	TSubclassOf<UUserWidget> PlayerOverlayClass;
	
private:
	UPROPERTY()
	TObjectPtr<UUserWidget> PlayerOverlay = nullptr;
};
