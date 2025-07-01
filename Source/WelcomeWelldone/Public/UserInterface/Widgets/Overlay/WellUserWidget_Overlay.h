// Copyright © 2025 bo6eR. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CommomTypes/WellCommonTypes.h"
#include "WellUserWidget_Overlay.generated.h"


UCLASS()
class WELCOMEWELLDONE_API UWellUserWidget_Overlay : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintImplementableEvent, Category=Attribute)
	void OnModifierStatusChanged(FGameplayTag Tag, const FGameplayMessage_ModifierChanged& Message);

	UFUNCTION(BlueprintImplementableEvent, Category=Attribute)
	void OnDeath(FGameplayTag Tag, const FGameplayMessage_BoolData& Message);
	
};
