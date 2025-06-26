// Copyright © 2025 bo6eR. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CommomTypes/WellCommonTypes.h"
#include "WellUserWidget_ActionButton.generated.h"


UCLASS()
class WELCOMEWELLDONE_API UWellUserWidget_ActionButton : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintImplementableEvent, Category=CountdownData)
	void OnFindSession(FGameplayTag Tag, const FGameplayMessage_BoolData& Message);
		
};
