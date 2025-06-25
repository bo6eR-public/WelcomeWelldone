// Copyright © 2025 bo6eR. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CommomTypes/WellCommonTypes.h"
#include "WellUserWidget_Lobby.generated.h"


UCLASS()
class WELCOMEWELLDONE_API UWellUserWidget_Lobby : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	
	UFUNCTION(BlueprintImplementableEvent, Category=CountdownData)
	void OnCountdownChanged(FGameplayTag Tag, const FGameplayMessage_IntData& Message);
	
};
